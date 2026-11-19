/* ResearchAdvisorLM — web client */

const $ = (sel) => document.querySelector(sel);
const $$ = (sel) => Array.from(document.querySelectorAll(sel));

const state = {
  paper: null,       // full paper object currently open
  depth: "short",
  compareIds: new Set(),
  project: null,     // discovery project currently open
  run: null,         // discovery run currently shown
  pollTimer: null,   // handle for the run-status poll
  navigated: false,  // true once the user picks a view themselves
  reviewIds: new Set(),
};

/* ---------------- helpers ---------------- */

const esc = (s) =>
  String(s ?? "").replace(/[&<>"']/g, (c) =>
    ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c])
  );

function toast(message, ms = 2600) {
  const el = $("#toast");
  el.textContent = message;
  el.classList.remove("hidden");
  clearTimeout(toast._t);
  toast._t = setTimeout(() => el.classList.add("hidden"), ms);
}

async function api(path, options = {}) {
  const response = await fetch(path, options);
  // A session can expire mid-visit. Send the user back to the gate once,
  // rather than letting every later action fail with its own error.
  if (response.status === 401 && !path.startsWith("/api/auth/")) {
    stopPolling();
    showGate(false);
    throw new Error("Your session expired. Sign in again.");
  }
  if (!response.ok) {
    let detail = `Request failed (${response.status})`;
    try {
      const body = await response.json();
      detail = body.detail || detail;
    } catch { /* non-JSON error body */ }
    throw new Error(detail);
  }
  return response.status === 204 ? null : response.json();
}

function show(view) {
  $$(".view").forEach((v) => v.classList.add("hidden"));
  $(`#view-${view}`).classList.remove("hidden");
  $$(".navbtn").forEach((b) => b.classList.toggle("active", b.dataset.nav === view));
  window.scrollTo({ top: 0, behavior: "smooth" });
}

function setStep(name, cls) {
  const step = $(`.step[data-step="${name}"]`);
  if (step) step.className = `step ${cls}`;
}

function resetSteps() {
  ["extract", "analyze", "search", "validate"].forEach((s) => setStep(s, ""));
}

/* ---------------- navigation ---------------- */

$$("[data-nav]").forEach((el) =>
  el.addEventListener("click", () => {
    const view = el.dataset.nav;
    state.navigated = true;
    // Leaving a run must stop its poll, or it keeps firing in the background.
    if (view !== "run") stopPolling();
    if (view === "library") loadLibrary();
    if (view === "compare") loadComparePicker();
    if (view === "discover") loadProjects();
    show(view);
  })
);

$$(".tab").forEach((tab) =>
  tab.addEventListener("click", () => {
    $$(".tab").forEach((t) => t.classList.remove("active"));
    tab.classList.add("active");
    $$(".tabpanel").forEach((p) => p.classList.add("hidden"));
    $(`#tab-${tab.dataset.tab}`).classList.remove("hidden");
  })
);

/* ---------------- upload ---------------- */

const dropzone = $("#dropzone");
const browseButton = $("#browse-btn");
const fileInput = $("#file-input");

browseButton.addEventListener("click", (e) => {
  e.preventDefault();
  e.stopPropagation();
  fileInput.value = "";
  fileInput.click();
});

dropzone.addEventListener("click", (e) => {
  if (e.target !== fileInput) {
    fileInput.value = "";
    fileInput.click();
  }
});

fileInput.addEventListener("change", () => {
  if (fileInput.files[0]) handleUpload(fileInput.files[0]);
});

["dragenter", "dragover"].forEach((evt) =>
  dropzone.addEventListener(evt, (e) => {
    e.preventDefault();
    dropzone.classList.add("drag");
  })
);
["dragleave", "drop"].forEach((evt) =>
  dropzone.addEventListener(evt, (e) => {
    e.preventDefault();
    dropzone.classList.remove("drag");
  })
);
dropzone.addEventListener("drop", (e) => {
  const file = e.dataTransfer.files[0];
  if (file) handleUpload(file);
});

async function handleUpload(file) {
  if (!file.name.toLowerCase().endsWith(".pdf")) {
    return showUploadError("Only PDF files are supported.");
  }

  $("#upload-error").classList.add("hidden");
  $("#progress").classList.remove("hidden");
  resetSteps();
  setStep("extract", "active");
  dropzone.style.opacity = ".45";

  const form = new FormData();
  form.append("file", file);

  try {
    setStep("extract", "done");
    setStep("analyze", "active");
    const result = await api("/api/upload", { method: "POST", body: form });
    await waitForAnalysis(result.paper_id);
    setStep("analyze", "done");

    // Open the paper immediately — grounding continues in the background.
    await openPaper(result.paper_id, result.warnings);

    setStep("search", "active");
    try {
      const grounding = await api(`/api/papers/${result.paper_id}/ground`, { method: "POST" });
      setStep("search", "done");
      setStep("validate", "done");
      state.paper.grounding = grounding;
      renderGaps();
      toast(`Checked ${grounding.corpus_size} papers from the literature`);
    } catch (err) {
      setStep("search", "");
      toast(`Gap grounding failed: ${err.message}`);
    }
  } catch (err) {
    showUploadError(err.message);
  } finally {
    dropzone.style.opacity = "1";
    fileInput.value = "";
    setTimeout(() => $("#progress").classList.add("hidden"), 1200);
  }
}

function showUploadError(message) {
  const el = $("#upload-error");
  el.textContent = message;
  el.classList.remove("hidden");
  $("#progress").classList.add("hidden");
  dropzone.style.opacity = "1";
}

/* ---------------- paper view ---------------- */

async function openPaper(paperId, warnings = []) {
  state.paper = await api(`/api/papers/${paperId}`);
  state.paper.warnings = warnings;
  renderPaper();
  show("paper");
}

function renderPaper() {
  const paper = state.paper;
  const analysis = paper.analysis || {};
  const meta = analysis.metadata || {};

  $("#paper-title").textContent = meta.title || paper.filename;

  const chips = [
    (meta.authors || []).slice(0, 3).join(", ") + ((meta.authors || []).length > 3 ? " et al." : ""),
    meta.year,
    meta.venue,
    meta.domain,
    `${paper.pages} pages`,
  ].filter(Boolean);
  $("#paper-meta").innerHTML = chips.map((c) => `<span class="chip">${esc(c)}</span>`).join("");

  $("#paper-warnings").innerHTML = (paper.warnings || [])
    .map((w) => `<div class="alert warn">${esc(w)}</div>`)
    .join("");

  renderSummary();
  renderGaps();
  renderChat();
}

function renderSummary() {
  // A paper whose analysis failed or was cut short by a restart would otherwise
  // render as an empty summary with no way forward.
  if (state.paper.analysis_status === "error") {
    $("#tab-summary").innerHTML = `
      <div class="alert error">
        ${esc(state.paper.analysis_error || "Analysis did not complete.")}
      </div>
      <div class="block">
        <h3>Recover this paper</h3>
        <p>The extracted text is already saved, so retrying costs one model call
        and no re-upload.</p>
        <button class="btn primary" id="retry-analysis">Retry analysis</button>
      </div>`;
    $("#retry-analysis").addEventListener("click", retryAnalysis);
    return;
  }
  if (state.paper.analysis_status === "pending") {
    $("#tab-summary").innerHTML = `
      <div class="alert info">Analysing this paper. This can take a few minutes
      on a free model — you can leave and come back.</div>`;
    return;
  }

  const analysis = state.paper.analysis || {};
  const summary = analysis.summary || {};
  const list = (items) =>
    (items || []).length
      ? `<ul>${items.map((i) => `<li>${esc(i)}</li>`).join("")}</ul>`
      : `<p class="quote">Not stated in this paper.</p>`;

  const depths = [
    ["one_line", "One line"],
    ["short", "Short"],
    ["detailed", "Detailed"],
    ["eli5", "Explain simply"],
  ];

  $("#tab-summary").innerHTML = `
    <div class="block">
      <h3>Summary</h3>
      <div class="depth">
        ${depths
          .map(
            ([key, label]) =>
              `<button data-depth="${key}" class="${state.depth === key ? "active" : ""}">${label}</button>`
          )
          .join("")}
      </div>
      <p id="summary-text">${esc(summary[state.depth] || summary.short || "")}</p>
    </div>

    <div class="block"><h3>Objective</h3><p>${esc(analysis.objective || "Not stated.")}</p></div>
    <div class="block"><h3>Method</h3><p>${esc(analysis.method || "Not stated.")}</p></div>

    <div class="block">
      <h3>Datasets</h3>
      <div class="tag-row">${
        (analysis.datasets || []).length
          ? analysis.datasets.map((d) => `<span class="tag">${esc(d)}</span>`).join("")
          : `<span class="quote">Not stated in this paper.</span>`
      }</div>
    </div>

    <div class="block"><h3>Contributions</h3>${list(analysis.contributions)}</div>
    <div class="block"><h3>Key findings</h3>${list(analysis.findings)}</div>
    <div class="block"><h3>Stated limitations</h3>${list(analysis.stated_limitations)}</div>
    <div class="block"><h3>Conclusion</h3><p>${esc(analysis.conclusion || "Not stated.")}</p></div>

    <div class="block">
      <h3>Keywords</h3>
      <div class="tag-row">${(analysis.metadata?.keywords || [])
        .map((k) => `<span class="tag">${esc(k)}</span>`)
        .join("")}</div>
    </div>
  `;

  $$("#tab-summary .depth button").forEach((btn) =>
    btn.addEventListener("click", () => {
      state.depth = btn.dataset.depth;
      renderSummary();
    })
  );
}

const VERDICT_LABEL = {
  open: "Still open",
  partial: "Partially addressed",
  addressed: "Addressed",
  unverified: "Unverified",
};

function renderGaps() {
  const paper = state.paper;
  const grounding = paper.grounding;
  const gaps = grounding?.gaps || paper.analysis?.gaps || [];

  $("#gap-count").textContent = gaps.length || "";

  if (grounding) {
    const open = gaps.filter((g) => g.verdict === "open").length;
    $("#grounding-banner").innerHTML = `
      <div class="alert info">
        <b>${open} of ${gaps.length}</b> directions still look open.
        Checked against <b>${grounding.corpus_size}</b> papers retrieved from OpenAlex,
        published from ${esc(grounding.paper_year || "n.d.")} onward.
        ${grounding.matched_paper ? "" : " The source paper was not found in OpenAlex, so citing-paper evidence is unavailable."}
        <br /><span class="ev-meta">This is a bounded corpus, not the whole literature — treat verdicts as evidence, not proof.</span>
      </div>`;
  } else {
    $("#grounding-banner").innerHTML = `
      <div class="alert warn">
        These gaps come from the paper alone and have <b>not</b> been checked against later
        literature yet. <button class="btn ghost" id="ground-now">Check literature now</button>
      </div>`;
    $("#ground-now")?.addEventListener("click", groundNow);
  }

  $("#gaps-list").innerHTML =
    gaps
      .map((gap, index) => {
        const verdict = gap.verdict || "unverified";
        const evidence = gap.evidence || [];
        return `
      <div class="gap ${verdict}">
        <div class="gap-top">
          <div class="gap-title">Gap ${index + 1} — ${esc(gap.title)}</div>
          <span class="verdict ${verdict}">${VERDICT_LABEL[verdict] || verdict}</span>
        </div>
        <div class="gap-meta">
          <span class="chip">${esc(gap.category || "general")}</span>
          <span class="chip">${gap.origin === "stated" ? "authors stated it" : "inferred by review"}</span>
          ${gap.confidence ? `<span class="chip">confidence: ${esc(gap.confidence)}</span>` : ""}
        </div>
        <p>${esc(gap.description)}</p>
        <p class="quote">In the paper: ${esc(gap.evidence_from_paper)}</p>
        ${gap.reasoning ? `<p><b>Literature check:</b> ${esc(gap.reasoning)}</p>` : ""}
        ${
          evidence.length
            ? `<details class="evidence">
                 <summary>${evidence.length} paper${evidence.length > 1 ? "s" : ""} supporting this verdict</summary>
                 ${evidence
                   .map(
                     (w) => `
                   <div class="ev-item">
                     <a href="${esc(w.url)}" target="_blank" rel="noopener">${esc(w.title)}</a>
                     <div class="ev-meta">${esc((w.authors || []).join(", "))} · ${esc(w.year || "n.d.")}
                       · ${w.citations} citations${w.venue ? " · " + esc(w.venue) : ""}</div>
                   </div>`
                   )
                   .join("")}
               </details>`
            : ""
        }
        ${gap.what_remains ? `<div class="remains"><b>What a new project could do:</b> ${esc(gap.what_remains)}</div>` : ""}
        ${
          verdict === "open" || verdict === "partial"
            ? `<div class="opp-actions">
                 <button class="btn ghost" data-propose="${esc(gap.id ?? index)}">Turn into a project</button>
               </div>
               <div class="proposal"></div>`
            : ""
        }
      </div>`;
      })
      .join("") || `<div class="empty">No gaps identified.</div>`;

  $$("#gaps-list [data-propose]").forEach((btn) =>
    btn.addEventListener("click", () => proposeProject(btn.dataset.propose, btn))
  );
}

async function groundNow() {
  const button = $("#ground-now");
  if (button) {
    button.disabled = true;
    button.textContent = "Searching literature…";
  }
  try {
    state.paper.grounding = await api(`/api/papers/${state.paper.id}/ground`, { method: "POST" });
    renderGaps();
    toast("Gap verdicts updated");
  } catch (err) {
    toast(err.message, 4000);
    if (button) {
      button.disabled = false;
      button.textContent = "Check literature now";
    }
  }
}

/* ---------------- chat ---------------- */

const SUGGESTIONS = [
  "Explain this paper in simple terms",
  "What dataset was used?",
  "What are the main results?",
  "What did they NOT test?",
  "How would I extend this?",
];

function renderChat() {
  const log = $("#chat-log");
  const messages = state.paper.messages || [];
  log.innerHTML = messages.length
    ? messages.map((m) => `<div class="msg ${m.role}">${esc(m.content)}</div>`).join("")
    : `<div class="msg assistant">Ask me anything about this paper. I answer from the paper text and name the section I used.</div>`;

  $("#chat-suggestions").innerHTML = SUGGESTIONS.map(
    (s) => `<button type="button">${esc(s)}</button>`
  ).join("");
  $$("#chat-suggestions button").forEach((b) =>
    b.addEventListener("click", () => sendMessage(b.textContent))
  );
}

$("#chat-form").addEventListener("submit", (e) => {
  e.preventDefault();
  const text = $("#chat-text").value.trim();
  if (text) sendMessage(text);
});

async function sendMessage(question) {
  const log = $("#chat-log");
  $("#chat-text").value = "";
  log.insertAdjacentHTML("beforeend", `<div class="msg user">${esc(question)}</div>`);
  log.insertAdjacentHTML("beforeend", `<div class="msg assistant thinking" id="thinking">Reading the paper…</div>`);
  log.scrollTop = log.scrollHeight;

  try {
    const result = await api("/api/chat", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ paper_id: state.paper.id, question }),
    });
    $("#thinking").outerHTML = `<div class="msg assistant">${esc(result.answer)}</div>`;
    state.paper.messages = [
      ...(state.paper.messages || []),
      { role: "user", content: question },
      { role: "assistant", content: result.answer },
    ];
  } catch (err) {
    $("#thinking").outerHTML = `<div class="msg assistant">⚠️ ${esc(err.message)}</div>`;
  }
  log.scrollTop = log.scrollHeight;
}

/* ---------------- export / delete ---------------- */

$("#export-btn").addEventListener("click", () => {
  window.open(`/api/papers/${state.paper.id}/export`, "_blank");
});

$("#delete-btn").addEventListener("click", async () => {
  if (!confirm("Delete this paper and its chat history?")) return;
  await api(`/api/papers/${state.paper.id}`, { method: "DELETE" });
  toast("Paper deleted");
  loadLibrary();
  show("library");
});

/* ---------------- library ---------------- */

function paperCard(paper, selectable = false) {
  const gapText = paper.grounded
    ? `<span>${paper.open_gaps} open / ${paper.total_gaps} gaps</span>`
    : `<span>${paper.total_gaps} gaps · not grounded</span>`;
  return `
    <div class="card ${selectable && state.compareIds.has(paper.id) ? "selected" : ""}" data-id="${paper.id}">
      <div class="card-title">${esc(paper.title || paper.filename)}</div>
      <div class="card-meta">
        ${paper.year ? `<span>${esc(paper.year)}</span>` : ""}
        <span>${paper.pages}p</span>
        ${gapText}
      </div>
    </div>`;
}

async function loadLibrary() {
  const papers = await api("/api/papers");
  $("#library-list").innerHTML = papers.length
    ? papers.map((p) => paperCard(p)).join("")
    : `<div class="empty">No papers yet. Upload one to get started.</div>`;
  $$("#library-list .card").forEach((card) =>
    card.addEventListener("click", () => openPaper(card.dataset.id))
  );

  loadReviewPicker(papers);
}

/* ---------------- compare ---------------- */

async function loadComparePicker() {
  const papers = await api("/api/papers");
  $("#compare-picker").innerHTML = papers.length
    ? papers.map((p) => paperCard(p, true)).join("")
    : `<div class="empty">Upload at least two papers to compare.</div>`;
  $$("#compare-picker .card").forEach((card) =>
    card.addEventListener("click", () => {
      const id = card.dataset.id;
      if (state.compareIds.has(id)) state.compareIds.delete(id);
      else if (state.compareIds.size >= 5) return toast("Maximum of 5 papers");
      else state.compareIds.add(id);
      card.classList.toggle("selected");
    })
  );
}

$("#run-compare").addEventListener("click", async () => {
  if (state.compareIds.size < 2) return toast("Select at least 2 papers");
  const button = $("#run-compare");
  button.disabled = true;
  button.textContent = "Comparing…";
  $("#compare-result").innerHTML = "";

  try {
    const result = await api("/api/compare", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ paper_ids: [...state.compareIds] }),
    });

    const headers = result.papers.map((p) => `<th>${esc(p.title)}</th>`).join("");
    const rows = result.rows
      .map(
        (row) =>
          `<tr><td>${esc(row.aspect)}</td>${row.values
            .map((v) => `<td>${esc(v)}</td>`)
            .join("")}</tr>`
      )
      .join("");

    $("#compare-result").innerHTML = `
      <table class="ctable"><thead><tr><th></th>${headers}</tr></thead><tbody>${rows}</tbody></table>
      <div class="block" style="margin-top:20px">
        <h3>Gaps none of them address</h3>
        <ul>${(result.shared_gaps || []).map((g) => `<li>${esc(g)}</li>`).join("")}</ul>
      </div>
      ${
        (result.contradictions || []).length
          ? `<div class="block"><h3>Contradictions</h3><ul>${result.contradictions
              .map((c) => `<li>${esc(c)}</li>`)
              .join("")}</ul></div>`
          : ""
      }
      <div class="block"><h3>Synthesis</h3><p>${esc(result.synthesis)}</p></div>`;
  } catch (err) {
    $("#compare-result").innerHTML = `<div class="alert error">${esc(err.message)}</div>`;
  } finally {
    button.disabled = false;
    button.textContent = "Compare selected";
  }
});

/* ---------------- import by URL ---------------- */

$("#import-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const url = $("#import-url").value.trim();
  if (!url) return;

  const button = $("#import-btn");
  button.disabled = true;
  button.textContent = "Importing…";
  $("#upload-error").classList.add("hidden");
  $("#progress").classList.remove("hidden");
  resetSteps();
  setStep("extract", "active");

  try {
    const result = await api("/api/import", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ url }),
    });
    setStep("extract", "done");
    setStep("analyze", "active");
    await waitForAnalysis(result.paper_id);
    setStep("analyze", "done");
    $("#import-url").value = "";
    await openPaper(result.paper_id, result.warnings);
    await groundInBackground(result.paper_id);
  } catch (err) {
    showUploadError(err.message);
  } finally {
    button.disabled = false;
    button.textContent = "Import";
    setTimeout(() => $("#progress").classList.add("hidden"), 1200);
  }
});

/* Analysis now finishes after the upload response, so the client waits for the
   paper row to leave "pending" instead of expecting a finished analysis. */
async function waitForAnalysis(paperId, tries = 240) {
  for (let i = 0; i < tries; i++) {
    const paper = await api(`/api/papers/${paperId}`);
    if (paper.analysis_status === "done") return paper;
    if (paper.analysis_status === "error") {
      throw new Error(paper.analysis_error || "Analysis failed.");
    }
    await new Promise((r) => setTimeout(r, 3000));
  }
  throw new Error("Analysis is taking unusually long. Check the library shortly.");
}

async function retryAnalysis() {
  const button = $("#retry-analysis");
  button.disabled = true;
  button.textContent = "Analysing…";
  try {
    await api(`/api/papers/${state.paper.id}/reanalyse`, { method: "POST" });
    await waitForAnalysis(state.paper.id);
    await openPaper(state.paper.id);
    toast("Analysis complete");
  } catch (err) {
    toast(err.message, 4000);
    button.disabled = false;
    button.textContent = "Retry analysis";
  }
}

async function groundInBackground(paperId) {
  setStep("search", "active");
  try {
    const grounding = await api(`/api/papers/${paperId}/ground`, { method: "POST" });
    setStep("search", "done");
    setStep("validate", "done");
    state.paper.grounding = grounding;
    renderGaps();
    toast(`Checked ${grounding.corpus_size} papers from the literature`);
  } catch (err) {
    setStep("search", "");
    toast(`Gap grounding failed: ${err.message}`);
  }
}

/* ---------------- citations ---------------- */

$("#cite-btn").addEventListener("click", async () => {
  const box = $("#citation-box");
  if (box.innerHTML) return (box.innerHTML = "");

  try {
    const styles = await api(`/api/papers/${state.paper.id}/citation`);
    box.innerHTML = `<div class="block">${Object.entries(styles)
      .map(
        ([name, text]) => `
        <div class="cite-row">
          <span class="cite-style">${esc(name)}</span>
          <span class="cite-text">${esc(text)}</span>
          <button class="btn ghost" data-copy="${esc(text)}">Copy</button>
        </div>`
      )
      .join("")}</div>`;

    $$("#citation-box [data-copy]").forEach((btn) =>
      btn.addEventListener("click", async () => {
        await navigator.clipboard.writeText(btn.dataset.copy);
        toast("Citation copied");
      })
    );
  } catch (err) {
    toast(err.message, 4000);
  }
});

/* ---------------- gap -> project proposal ---------------- */

async function proposeProject(gapId, button) {
  const holder = button.closest(".gap").querySelector(".proposal");
  button.disabled = true;
  button.textContent = "Drafting…";
  try {
    const p = await api(`/api/papers/${state.paper.id}/topic`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ gap_id: gapId }),
    });
    const list = (items) =>
      `<ul>${(items || []).map((i) => `<li>${esc(i)}</li>`).join("")}</ul>`;
    holder.innerHTML = `
      <div class="block">
        <h3>${esc(p.title || "Project proposal")}</h3>
        <h4>Problem</h4><p>${esc(p.problem || "")}</p>
        <h4>Why it is open</h4><p>${esc(p.why_it_is_open || "")}</p>
        <h4>Proposed approach</h4><p>${esc(p.proposed_approach || "")}</p>
        <h4>Datasets</h4><div class="tag-row">${(p.datasets || [])
          .map((d) => `<span class="tag">${esc(d)}</span>`)
          .join("")}</div>
        <h4>Evaluation plan</h4><p>${esc(p.evaluation_plan || "")}</p>
        <h4>Expected contribution</h4><p>${esc(p.expected_contribution || "")}</p>
        <h4>Prerequisites</h4>${list(p.prerequisites)}
        <h4>Risks</h4>${list(p.risks)}
        ${p.difficulty ? `<span class="chip">difficulty: ${esc(p.difficulty)}</span>` : ""}
      </div>`;
    button.textContent = "Proposal drafted";
  } catch (err) {
    toast(err.message, 4000);
    button.disabled = false;
    button.textContent = "Turn into a project";
  }
}

/* ---------------- library chat ---------------- */

$("#libchat-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const question = $("#libchat-text").value.trim();
  if (!question) return;

  const button = $("#libchat-btn");
  const answer = $("#libchat-answer");
  button.disabled = true;
  button.textContent = "Thinking…";
  answer.innerHTML = `<div class="msg assistant thinking">Reading your library…</div>`;

  try {
    const result = await api("/api/chat/library", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ question }),
    });
    answer.innerHTML = `
      <div class="msg assistant">${esc(result.answer)}</div>
      <p class="ev-meta">Answered from ${result.papers_searched} paper${
        result.papers_searched === 1 ? "" : "s"
      } in your library.</p>`;
  } catch (err) {
    answer.innerHTML = `<div class="alert error">${esc(err.message)}</div>`;
  } finally {
    button.disabled = false;
    button.textContent = "Ask";
  }
});

/* ---------------- related-work review ---------------- */

function loadReviewPicker(papers) {
  $("#review-picker").innerHTML = papers.length
    ? papers
        .map(
          (p) => `
        <div class="card ${state.reviewIds.has(p.id) ? "selected" : ""}" data-id="${p.id}">
          <div class="card-title">${esc(p.title || p.filename)}</div>
          <div class="card-meta">${
            p.year ? `<span>${esc(p.year)}</span>` : ""
          }<span>${p.pages}p</span></div>
        </div>`
        )
        .join("")
    : `<div class="empty">Add at least two papers to generate a review.</div>`;

  $$("#review-picker .card").forEach((card) =>
    card.addEventListener("click", () => {
      const id = card.dataset.id;
      if (state.reviewIds.has(id)) state.reviewIds.delete(id);
      else if (state.reviewIds.size >= 10) return toast("Maximum of 10 papers");
      else state.reviewIds.add(id);
      card.classList.toggle("selected");
    })
  );
}

$("#review-btn").addEventListener("click", async () => {
  if (state.reviewIds.size < 2) return toast("Select at least 2 papers");
  const button = $("#review-btn");
  button.disabled = true;
  button.textContent = "Writing…";
  $("#review-result").innerHTML = "";

  try {
    const result = await api("/api/review", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ paper_ids: [...state.reviewIds] }),
    });
    const themes = (result.themes || [])
      .map(
        (t) => `
        <div class="block">
          <h3>${esc(t.name || "Theme")}</h3>
          <p>${esc(t.discussion || "")}</p>
          ${
            (t.papers || []).length
              ? `<div class="tag-row">${t.papers
                  .map((p) => `<span class="tag">${esc(p)}</span>`)
                  .join("")}</div>`
              : ""
          }
        </div>`
      )
      .join("");
    $("#review-result").innerHTML = `
      ${
        result.introduction
          ? `<div class="block"><h3>Introduction</h3><p>${esc(result.introduction)}</p></div>`
          : ""
      }
      ${themes}`;
  } catch (err) {
    $("#review-result").innerHTML = `<div class="alert error">${esc(err.message)}</div>`;
  } finally {
    button.disabled = false;
    button.textContent = "Generate from selected";
  }
});

/* ---------------- discovery ---------------- */

// The pipeline emits more steps than the checklist shows; each checklist row is
// marked done once its own step has been seen in the trace.
const RUN_STEPS = [
  "question_understood",
  "papers_discovered",
  "papers_selected",
  "papers_analysed",
  "gaps_synthesised",
  "gaps_selected_for_validation",
  "opportunities_ranked",
];

const SCORE_LABEL = {
  unresolved: "Still unresolved",
  recurrence: "Recurs across papers",
  evidence_strength: "Evidence strength",
  not_contradicted: "Not contradicted",
  confidence: "Validator confidence",
};

const GOAL_LABEL = {
  gaps: "Find research gaps",
  review: "Literature review",
  opportunities: "Research opportunities",
};

function resetRunSteps() {
  RUN_STEPS.forEach((s) => setStep(s, ""));
}

function renderRunProgress(run) {
  const seen = new Set((run.trace || []).map((t) => t.step));
  const finished = run.status !== "running";
  let active = false;

  RUN_STEPS.forEach((name) => {
    if (seen.has(name)) return setStep(name, "done");
    if (!finished && !active) {
      active = true;
      return setStep(name, "active");
    }
    setStep(name, "");
  });
}

function evidenceList(items) {
  return (
    (items || [])
      .map(
        (w) => `
      <div class="ev-item">
        <a href="${esc(w.url)}" target="_blank" rel="noopener">${esc(w.title)}</a>
        <div class="ev-meta">${esc((w.authors || []).join(", "))} · ${esc(w.year || "n.d.")}
          · ${w.citations ?? 0} citations${w.venue ? " · " + esc(w.venue) : ""}</div>
      </div>`
      )
      .join("") || `<div class="ev-item ev-meta">Nothing found.</div>`
  );
}

function scoreBlock(opportunity) {
  const breakdown = opportunity.score_breakdown || {};
  const parts = Object.entries(breakdown)
    .map(
      ([key, value]) => `
      <div class="score-part">
        <span>${esc(SCORE_LABEL[key] || key)}</span>
        <span class="score-track"><span class="score-fill" style="width:${Math.round(
          Math.max(0, Math.min(1, Number(value) || 0)) * 100
        )}%"></span></span>
        <span class="score-num">${(Number(value) || 0).toFixed(2)}</span>
      </div>`
    )
    .join("");

  if (!parts) return "";

  return `
    <div class="score">
      <div class="score-top">
        <span>Why it ranked here</span>
        <span class="score-value">${
          Number.isFinite(Number(opportunity.system_score))
            ? Number(opportunity.system_score).toFixed(1)
            : "—"
        }</span>
      </div>
      <div class="score-parts">${parts}</div>
      <p class="score-note">
        A system-generated score out of 100, from the signals above — not a measure of
        scientific merit.
      </p>
    </div>`;
}

function opportunityCard(opportunity, index) {
  const verdict = opportunity.verdict || "unverified";
  const supporting = opportunity.supporting_evidence || [];
  const contradicting = opportunity.contradicting_evidence || [];
  const frequency = opportunity.frequency ?? 1;
  // The pipeline reports a failed or skipped validation by writing the reason
  // into `reasoning` and leaving the verdict unverified. Showing that as a
  // verdict would dress up an infrastructure failure as a research finding.
  const validationFailed =
    verdict === "unverified" &&
    /^(Validation LLM call failed|Validation skipped|No later literature)/.test(
      opportunity.reasoning || ""
    );

  return `
    <div class="gap opp ${verdict}" style="animation-delay:${Math.min(index, 8) * 45}ms">
      <div class="opp-rank">Opportunity ${index + 1}</div>
      <div class="gap-top">
        <div class="gap-title">${esc(opportunity.title)}</div>
        <span class="verdict ${verdict}">${VERDICT_LABEL[verdict] || verdict}</span>
      </div>
      <div class="gap-meta">
        <span class="chip">${esc(opportunity.category || "general")}</span>
        <span class="chip">seen in ${frequency} paper${frequency === 1 ? "" : "s"}</span>
        <span class="chip">confidence: ${esc(opportunity.confidence || "low")}</span>
        <span class="chip">feasibility: ${esc(opportunity.feasibility || "medium")}</span>
      </div>

      <p>${esc(opportunity.description)}</p>
      ${opportunity.why_it_matters ? `<p><b>Why it matters:</b> ${esc(opportunity.why_it_matters)}</p>` : ""}
      ${
        validationFailed
          ? `<div class="alert warn">The challenge could not run, so this gap is
               <b>unproven</b> rather than confirmed open.
               <br /><span class="ev-meta">${esc(opportunity.reasoning)}</span></div>`
          : opportunity.reasoning
            ? `<p><b>Challenge result:</b> ${esc(opportunity.reasoning)}</p>`
            : ""
      }
      ${opportunity.novelty_assessment ? `<p class="quote">${esc(opportunity.novelty_assessment)}</p>` : ""}

      <details class="evidence">
        <summary>${supporting.length} paper${supporting.length === 1 ? "" : "s"} supporting this gap</summary>
        ${evidenceList(supporting)}
      </details>
      ${
        contradicting.length
          ? `<details class="evidence contra">
               <summary>${contradicting.length} paper${
                 contradicting.length === 1 ? "" : "s"
               } arguing it is already addressed</summary>
               ${evidenceList(contradicting)}
             </details>`
          : ""
      }

      ${scoreBlock(opportunity)}

      ${
        opportunity.suggested_next_step || opportunity.what_remains
          ? `<div class="remains"><b>Suggested next step:</b> ${esc(
              opportunity.suggested_next_step || opportunity.what_remains
            )}</div>`
          : ""
      }

      <div class="opp-actions">
        <button class="btn ghost" data-challenge="${esc(opportunity.id)}">Challenge this gap</button>
      </div>
    </div>`;
}

function renderRun(run) {
  $("#run-topic").textContent = run.query;

  const chips = [
    GOAL_LABEL[run.goal] || run.goal,
    run.status === "running" ? "Running…" : run.status === "done" ? "Complete" : "Stopped",
    run.llm_calls ? `${run.llm_calls} model calls` : null,
  ].filter(Boolean);
  $("#run-meta").innerHTML = chips.map((c) => `<span class="chip">${esc(c)}</span>`).join("");

  renderRunProgress(run);

  const error = $("#run-error");
  if (run.status === "error" && run.error) {
    error.textContent = run.error;
    error.classList.remove("hidden");
  } else {
    error.classList.add("hidden");
  }

  const result = run.result || {};
  const opportunities = result.opportunities || [];

  if (run.status === "running") {
    $("#run-banner").innerHTML = `
      <div class="alert info">Reading the literature. This takes a few minutes —
      you can leave and come back; the run keeps going.</div>`;
  } else if (opportunities.length) {
    $("#run-banner").innerHTML = `
      <div class="alert info">
        <b>${opportunities.length}</b> opportunit${opportunities.length === 1 ? "y" : "ies"}
        survived challenge, from <b>${result.candidate_gaps ?? 0}</b> candidate gaps across
        <b>${result.papers_analysed ?? 0}</b> analysed papers.
        <br /><span class="ev-meta">A bounded corpus, not the whole literature — nothing here
        is novel merely because this search did not find it.</span>
      </div>`;
  } else {
    $("#run-banner").innerHTML = "";
  }

  $("#opportunities").innerHTML = opportunities.length
    ? opportunities.map(opportunityCard).join("")
    : run.status === "running"
      ? ""
      : `<div class="empty">No opportunities survived validation for this topic.</div>`;

  $$("#opportunities [data-challenge]").forEach((btn) =>
    btn.addEventListener("click", () => challengeGap(run.id, btn.dataset.challenge, btn))
  );

  renderRunPapers(run, result.papers || []);
}

function renderRunPapers(run, papers) {
  const holder = $("#run-papers");
  if (!papers.length) return (holder.innerHTML = "");

  holder.innerHTML = `
    <div class="view-head projects-head"><h2>Papers read</h2></div>
    <p class="sub">Discovery reads abstracts. Save one to your library to chat with
    it, ground its gaps, or export it.</p>
    <div class="cards">
      ${papers
        .map(
          (p, index) => `
        <div class="card" data-save="${index}">
          <div class="card-title">${esc(p.title || "Untitled")}</div>
          <div class="card-meta">
            ${p.year ? `<span>${esc(p.year)}</span>` : ""}
            <span>${p.cited_by_count ?? 0} citations</span>
            <span class="linkbtn">Save to library</span>
          </div>
        </div>`
        )
        .join("")}
    </div>`;

  $$("#run-papers [data-save]").forEach((card) =>
    card.addEventListener("click", () => savePaperToLibrary(run.id, card))
  );
}

async function savePaperToLibrary(runId, card) {
  const label = card.querySelector(".linkbtn");
  label.textContent = "Analysing…";
  try {
    const result = await api(`/api/discover/runs/${runId}/save-paper`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ paper_index: Number(card.dataset.save) }),
    });
    label.textContent = "Saved";
    toast(`"${result.title}" added to your library`);
  } catch (err) {
    label.textContent = "Save to library";
    toast(err.message, 4000);
  }
}

$("#run-export-btn").addEventListener("click", () => {
  if (!state.run) return toast("Nothing to export yet");
  window.open(`/api/discover/runs/${state.run.id}/export?format=md`, "_blank");
});

async function pollRun(runId) {
  const { run } = await api(`/api/discover/runs/${runId}`);
  state.run = run;
  renderRun(run);

  if (run.status === "running") {
    state.pollTimer = setTimeout(() => pollRun(runId), 2500);
  } else if (run.status === "done") {
    toast(`Discovery complete — ${run.llm_calls} model calls`);
  }
}

function stopPolling() {
  clearTimeout(state.pollTimer);
}

async function openRun(runId, project) {
  stopPolling();
  if (project) state.project = project;
  resetRunSteps();
  $("#run-error").classList.add("hidden");
  $("#opportunities").innerHTML = "";
  $("#run-banner").innerHTML = "";
  show("run");
  await pollRun(runId);
}

async function challengeGap(runId, gapId, button) {
  button.disabled = true;
  button.textContent = "Searching for counter-evidence…";
  try {
    const { run } = await api(`/api/discover/runs/${runId}/challenge`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ gap_id: gapId }),
    });
    state.run = run;
    renderRun(run);
    toast("Verdict re-checked against the literature");
  } catch (err) {
    toast(err.message, 4000);
    button.disabled = false;
    button.textContent = "Challenge this gap";
  }
}

$("#discover-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const topic = $("#discover-topic").value.trim();
  const errorBox = $("#discover-error");
  errorBox.classList.add("hidden");

  if (!topic) {
    errorBox.textContent = "Enter a research topic to start.";
    return errorBox.classList.remove("hidden");
  }

  const button = $("#start-discovery");
  button.disabled = true;
  button.textContent = "Starting…";

  try {
    // Browsers restore form state across reloads, so never assume the HTML
    // default survived.
    const goal = document.querySelector('input[name="goal"]:checked')?.value || "gaps";
    const { project } = await api("/api/discover/projects", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ topic, goal }),
    });
    const { run } = await api(`/api/discover/projects/${project.id}/runs`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({}),
    });
    $("#discover-topic").value = "";
    await openRun(run.id, project);
  } catch (err) {
    errorBox.textContent = err.message;
    errorBox.classList.remove("hidden");
  } finally {
    button.disabled = false;
    button.textContent = "Start discovery";
  }
});

$("#rerun-btn").addEventListener("click", async () => {
  if (!state.project) return;
  const button = $("#rerun-btn");
  button.disabled = true;
  button.textContent = "Starting…";
  try {
    const { run } = await api(`/api/discover/projects/${state.project.id}/runs`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({}),
    });
    await openRun(run.id);
  } catch (err) {
    toast(err.message, 4000);
  } finally {
    button.disabled = false;
    button.textContent = "Run again";
  }
});

$("#delete-project-btn").addEventListener("click", async () => {
  if (!state.project) return;
  if (!confirm("Delete this research project and every run inside it?")) return;
  stopPolling();
  await api(`/api/discover/projects/${state.project.id}`, { method: "DELETE" });
  toast("Project deleted");
  await loadProjects();
  show("discover");
});

async function loadProjects() {
  // Browsers restore radio state across reloads, which can leave a goal
  // selected that the user never picked. Re-assert the documented default.
  $('input[name="goal"][value="gaps"]').checked = true;

  const { projects } = await api("/api/discover/projects");
  $("#projects-list").innerHTML = projects.length
    ? projects
        .map(
          (p) => `
        <div class="card" data-project="${esc(p.id)}">
          <div class="card-title">${esc(p.topic)}</div>
          <div class="card-meta">
            <span>${p.run_count} run${p.run_count === 1 ? "" : "s"}</span>
            ${p.last_status ? `<span>${esc(p.last_status)}</span>` : "<span>not run yet</span>"}
          </div>
        </div>`
        )
        .join("")
    : `<div class="empty">No saved research yet. Ask a question above to start.</div>`;

  $$("#projects-list .card").forEach((card) =>
    card.addEventListener("click", () => openProject(card.dataset.project))
  );
}

async function openProject(projectId) {
  const { project, runs } = await api(`/api/discover/projects/${projectId}`);
  state.project = project;
  if (runs.length) return openRun(runs[0].id, project);

  // A saved question that was never run: show it ready to go, not a dead end.
  stopPolling();
  resetRunSteps();
  $("#run-topic").textContent = project.topic;
  $("#run-meta").innerHTML = "";
  $("#run-error").classList.add("hidden");
  $("#run-banner").innerHTML = `<div class="alert info">This project has no runs yet.</div>`;
  $("#opportunities").innerHTML = "";
  show("run");
}

/* ---------------- auth ---------------- */

// "register" when the deployment has no accounts yet, otherwise "login".
let authMode = "login";

function setAuthMode(mode) {
  authMode = mode;
  const registering = mode === "register";
  $("#auth-title").textContent = registering ? "Create your account" : "Sign in";
  $("#auth-sub").textContent = registering
    ? "Your library and research stay private to this account."
    : "Your library and research are private to your account.";
  $("#auth-submit").textContent = registering ? "Create account" : "Sign in";
  $("#auth-switch-text").textContent = registering
    ? "Already have an account?"
    : "No account yet?";
  $("#auth-switch").textContent = registering ? "Sign in" : "Create one";
  $("#auth-password").setAttribute(
    "autocomplete",
    registering ? "new-password" : "current-password"
  );
  $("#auth-error").classList.add("hidden");
}

$("#auth-switch").addEventListener("click", () =>
  setAuthMode(authMode === "login" ? "register" : "login")
);

function showGate(setupNeeded) {
  setAuthMode(setupNeeded ? "register" : "login");
  if (setupNeeded) {
    $("#auth-sub").textContent =
      "No account exists yet. The first account created owns this library.";
  }
  $("#auth-gate").classList.remove("hidden");
  $("#main").classList.add("hidden");
  document.querySelector(".topbar").classList.add("hidden");
}

function hideGate(user) {
  $("#auth-gate").classList.add("hidden");
  $("#main").classList.remove("hidden");
  document.querySelector(".topbar").classList.remove("hidden");
  $("#nav-user").textContent = user?.email || "";
}

$("#auth-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const email = $("#auth-email").value.trim();
  const password = $("#auth-password").value;
  const button = $("#auth-submit");
  const error = $("#auth-error");

  error.classList.add("hidden");
  button.disabled = true;
  button.textContent = authMode === "register" ? "Creating…" : "Signing in…";

  try {
    const result = await api(`/api/auth/${authMode}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ email, password }),
    });
    $("#auth-password").value = "";
    hideGate(result.user);

    const claimed = result.claimed;
    if (claimed && (claimed.papers || claimed.projects)) {
      toast(
        `Welcome. ${claimed.papers} paper(s) and ${claimed.projects} project(s) are now yours.`
      );
    }
    await startApp();
  } catch (err) {
    error.textContent = err.message;
    error.classList.remove("hidden");
    button.disabled = false;
    setAuthMode(authMode);
  }
});

$("#signout-btn").addEventListener("click", async () => {
  stopPolling();
  try {
    await api("/api/auth/logout", { method: "POST" });
  } catch {
    /* signing out locally matters more than the round trip succeeding */
  }
  window.location.reload();
});

/* ---------------- boot ---------------- */

async function startApp() {
  try {
    const health = await api("/api/health");
    if (!health.key_configured) {
      showUploadError(
        "No LLM_API_KEY configured on the server. Set it in your .env file or Railway variables."
      );
    }
    await loadLibrary();
  } catch {
    /* server not reachable yet */
  }
  // loadLibrary is awaited, so the user may already have navigated by now;
  // landing them back on Upload would undo a click they made deliberately.
  if (!state.navigated) show("upload");
}

(async function boot() {
  try {
    const { user, setup_needed: setupNeeded } = await api("/api/auth/me");
    if (!user) return showGate(setupNeeded);
    hideGate(user);
  } catch {
    // Server unreachable: show the gate rather than an empty shell that will
    // 401 on every action.
    return showGate(false);
  }
  await startApp();
})();
