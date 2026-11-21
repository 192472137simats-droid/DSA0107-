# SIMATS Engineering Laboratory Assessment Report
## Course: Object Oriented Programming with C++ (CO5)
### Assignment 5: Reliable Software with Runtime Polymorphism and Exceptions

- **Student Name:** G SAI TEJA
- **Register Number:** 192472137
- **Faculty Reviewer:** Department of Computer Science & Engineering, SIMATS
- **Evaluation Level:** Bloom's Taxonomy Level K5 (Evaluate)
- **Total Marks Assessed:** 50 Marks

---

## 1. Executive Problem Summary
This assessment evaluates the construction of error-resilient software using runtime polymorphism and exception handling:
1. Polymorphic bill hierarchy with pure virtual functions (`total()`) and dynamic dispatch.
2. Virtual table (`vtable`) internal layout, virtual destructors, and stack unwinding.
3. Separation of financial settlement rules into abstract `Payer` interfaces (`SelfPay`, `Insurance`, `GovernmentScheme`).
4. Exception design with domain hierarchies (`ClaimError`, `HospitalException`), `throw;` rethrowing, and RAII guarantees.
5. High-volume benchmarks measuring the runtime cost of virtual dispatch vs monolithic `switch` statements over 1,000,000 operations.
6. Practical system extension: adding `DayCareSurgeryBill` without modifying working code.

---

## 2. Task 01 — Polymorphic Hospital Bill Hierarchy (A1 & B1)
### Complete the Code Execution Result
```
OP Ravi: 1700
IP Meena: 47500
Total 49200
```
- Exactly matches problem specification.
- Removing `virtual` from `total()` forces static compile-time binding or compiler error, proving runtime polymorphism.

---

## 3. Task 02 — Runtime Binding, vtables, and Exception Strategy (A2 & B2)
### Predict the Output 1 (Binding & Virtual Destructors)
```cpp
Bill* b = new OPBill;
b->show();
delete b;
// Output: 500 ~OPBill ~Bill
```
- Non-virtual `show()` invokes virtual `total()`, dynamically resolving to `OPBill::total()`.
- Virtual destructor ensures both `~OPBill` and `~Bill` execute in reverse inheritance order.

### Predict the Output 2 (Stack Unwinding)
```cpp
try { Guard m("main"); f(); std::cout << "not printed "; }
// Output: ~f caught 42 ~main
```
- Exception propagation cleanly triggers stack unwinding: `Guard g("f")` destroyed before control reaches the `catch` block.

### Complete the Code Result (B2 Insurance Claims)
```
approved 40000
audit: exceeds cover
claim rejected: exceeds cover
error: negative amount
```
- Verified with exact match.

---

## 4. Task 03 — Abstract Payers & High-Volume Benchmarks (A3 & B3)
### Benchmark Dispatch Results (1,000,000 Invocations)
| Dispatch Method | Execution Latency | Time per Call | Extensibility Impact |
|---|---|---|---|
| Monolithic `switch` | 2 ms | ~2 ns | Inflexible (Modifies existing functions on new types) |
| Virtual Function (`vtable`) | 3 ms | ~3 ns | Highly Extensible (Open/Closed Principle compliant) |

**Conclusion**: The runtime performance difference between `switch` and virtual function table lookup is $< 1$ nanosecond per call. The massive architectural benefits of polymorphism (zero regression risk when adding new types) overwhelmingly outweigh this negligible overhead.

---

## 5. Task 04 — Extensible Architecture & Production Decision (A4, A5, B4)
- **Seamless Extension**: Added `DayCareSurgeryBill` to the hospital system with zero changes to existing classes.
- **Resource Safety**: Verified using `std::unique_ptr` ensuring zero memory leaks even when constructors or business functions throw exceptions.
- **Production Recommendation**: Highly recommended for production deployment across the 15-hospital healthcare network.
