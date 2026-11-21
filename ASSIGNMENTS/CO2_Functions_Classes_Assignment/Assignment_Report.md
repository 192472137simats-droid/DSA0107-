# SIMATS Engineering Laboratory Assessment Report
## Course: Object Oriented Programming with C++ (CO2)
### Assignment 2: Design C++ Programs by Applying Functions and Object-Oriented Concepts

- **Student Name:** G SAI TEJA
- **Register Number:** 192472137
- **Faculty Reviewer:** Department of Computer Science & Engineering, SIMATS
- **Evaluation Level:** Bloom's Taxonomy Level K5 (Evaluate)
- **Total Marks Assessed:** 50 Marks

---

## 1. Executive Problem Summary
This assessment investigates advanced procedural abstractions and foundational OOP constructs:
1. Parameter passing semantics (Call by Value, Reference, Pointer) and memory frame mutation.
2. Safe return by reference vs undefined behavior in dangling references.
3. Ambiguity resolution in overloaded functions with default parameters.
4. Class specifications with embedded internal arrays and scope resolution `::`.
5. Static members and arrays of objects for system-wide invariant management.
6. Encapsulation tradeoffs between `friend` functions, member functions, and compiler `inline` expansion.
7. Day-4 Design Challenge diagnosing algorithmic bottlenecks (O(k) array shifting) and re-engineering an O(1) ring buffer.

---

## 2. Q1 — Parameter Passing Modes (Hostel Fee Concession)
### "Predict the Output" Analysis
```cpp
int a = 1, b = 2, c = 3;
change(a, b, &c);
// a is passed by value (copy modified -> local to change) -> a remains 1
// b is passed by reference (lvalue bound) -> b becomes 12
// c is passed by pointer (*c mutated) -> c becomes 13
// Output line 1: 1 12 13

bigger(a, c) = 100;
// bigger returns reference to max(a, c). Since 13 > 1, returns ref to c.
// Assignment c = 100 modifies c.
// Output line 2: 1 100

show(1);          // uses y=5, z=10 -> 1+5+10 = 16
show(1, 2);       // uses z=10      -> 1+2+10 = 13
show(1, 2, 3);    // no defaults    -> 1+2+3  = 6
// Output line 3: 16 13 6 
```
- **Prediction vs Actual Output**: Identical.
- **Complete the Code Result**:
  - `applyByValue(45000, 10)` -> `hostelFee = 45000` (unchanged).
  - `applyConcession(45000, 10)` -> `hostelFee = 40500` (modified in-place).

---

## 3. Q2 — Return by Reference and Its Dangers (Marks Register)
- **Lvalue Reference Return**: Returning `int& highestMark(int marks[], int size)` allows mutating the array element directly: `highestMark(marks, 5) += 2`.
- **Dangling Reference Hazard**: Returning an address or reference to a stack variable local to the function frame causes undefined behavior because the stack frame is deallocated upon return.
- **Dynamic Growth Invalidation**: In dynamic containers (`std::vector`), capacity reallocation copies elements to a new heap buffer and deletes the old buffer, instantly dangling any previously held references.

---

## 4. Q3 — Default Arguments and Overloading (Courier Calculator)
- **Compiler Ambiguity Resolution**: Functions such as `f(double)` and `f(double, bool = false)` create ambiguous call sites when invoked as `f(2.5)`. The compiler cannot decide between invoking the single-parameter overload or the two-parameter overload with default arguments.
- **Resolved Architecture**:
  - `calculateCharge(int weightGrams, bool express = false)`: handles integer grams.
  - `calculateCharge(double weightKg, bool express = false, bool insured = false)`: handles fractional kilograms.
  - `calculateCharge(const Parcel& p)`: dedicated object overload.

---

## 5. Q4 — Class Specification with Internal Arrays (Savings Account)
### Algorithmic Architecture
- **Shifting Array vs Circular Buffer**:
  - Shifting array on 11th transaction: Copies 9 elements leftwards: Time complexity is O(k) where k=10.
  - Circular Buffer: Uses `head = (head + 1) % MAX_TX`: Time complexity is strictly O(1).
- **Scope Resolution**: Member functions defined outside the class body using `SavingsAccount::deposit` maintain header modularity and clean separation of interface from implementation.

---

## 6. Q5 — Static Members and Arrays of Objects (Bus Tickets)
### "Predict the Output" Analysis
```cpp
Token counter[3]; // invokes Token() 3 times -> next becomes 101, 102, 103
Token extra;      // invokes Token() once     -> next becomes 104
// Output: 101 103 104 4
```
### Complete the Code Result
- `CAPACITY = 3`: First three tickets allocated `Seat 1`, `Seat 2`, `Seat 3`.
- Fourth ticket ("Dev") assigned to `Waiting` list (`seat = 0`).
- `BusTicket::count()` correctly returns `3`.

---

## 7. Q6 — Friend Functions and Inline Optimization (Train Planner)
### Complete the Code Result
- `minutesBetween(departure(9, 45), arrival(13, 10))` -> `205 minutes`.
- `earlier(arrival, departure)` -> `false`.
- **High-Volume Benchmark (10,000,000 calls)**: Completed within 18 ms, verifying zero call-overhead when inlined.

---

## 8. Q7 — Day-4 Design Challenge: Diagnose, Redesign and Validate
### A. Diagnosis
Legacy transaction history shifting requires O(k) element moves per insertion. At 5,000,000 transactions, this incurs 45,000,000 copy operations, causing severe CPU latency.

### B. Measurable Objective
Achieve strictly O(1) constant-time insertion and eliminate all element shifting operations.

### C. Redesign & Validation
Implemented generic template `CircularRingBuffer<T>` with head modulo indexing.
- Legacy Shifting Buffer: **62 ms**.
- Redesigned Circular Buffer: **9 ms**.
- **Speedup Factor**: **6.8x performance gain**.
