# SIMATS Engineering Laboratory Assessment Report
## Course: Object Oriented Programming with C++ (CO3)
### Assignment 3: Construct C++ Programs Using Constructors, Destructors and Operator Overloading

- **Student Name:** G SAI TEJA
- **Register Number:** 192472137
- **Faculty Reviewer:** Department of Computer Science & Engineering, SIMATS
- **Evaluation Level:** Bloom's Taxonomy Level K5 (Evaluate)
- **Total Marks Assessed:** 50 Marks

---

## 1. Executive Problem Summary
This laboratory assessment focuses on managing object lifecycle and syntactic polymorphism in C++:
1. Constructor overloading, delegation, member initializer lists, and explicit constructor conversion guards.
2. Shallow vs deep copy semantics, heap pointer aliasing, and the formal **Rule of Three**.
3. Dynamic multidimensional memory allocation (`new[]`/`delete[]`), lifecycle tracing, and **Resource Acquisition Is Initialization (RAII)**.
4. Overloading arithmetic (`+`, `-`, `+=`), relational (`==`, `<`), and stream (`<<`) operators.
5. Prefix vs postfix increment (`++`) signature disambiguation (`int` dummy parameter) and checked array subscripting (`operator[]`).
6. Operator overloading constraints (inviolable precedence, arity, non-overloadable operators like `sizeof`, `::`, `.`, `?:`).
7. First-principles engineering of a `Complex` number impedance calculator for series and parallel AC circuits with zero-division safety.

---

## 2. Q1 — Constructors & Constructor Overloading (Library Books)
- **Member Initializer Lists**: Direct initialization in the constructor header executes before the body runs, avoiding default construction followed by assignment.
- **Constructor Delegation**: Delegating `Book()` and `Book(title)` to the primary 3-parameter constructor `Book(isbn, title, copies)` consolidates invariant validation in a single place.
- **Explicit Keyword**: Declaring `explicit Book(const std::string& title)` prevents unintended implicit conversions where raw strings are silently converted into `Book` objects in function arguments.

---

## 3. Q2 — Deep Copies & Rule of Three (Hospital Prescriptions)
### "Complete the Code" Implementation
- **TODO 1 (Deep Copy Constructor)**: Allocates an independent array `new std::string[other.n]` and performs deep element copying, preventing shared pointer aliasing.
- **TODO 2 (Copy Assignment Operator)**: Guards against self-assignment (`if (this != &other)`), deallocates existing memory via `delete[] medicines`, allocates a fresh buffer, and returns `*this`.
- **TODO 3 (Destructor)**: Frees dynamically allocated memory via `delete[] medicines`.
- **Expected vs Actual Output**:
  ```
  Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS
  ```
  Verified with exact match.

---

## 4. Q3 — Dynamic Constructors, Destructors & RAII (Auditorium)
### "Predict the Output" Lifecycle Trace
```cpp
Logger a("A");    // Outputs: +A 
{
    Logger b("B"); // Outputs: +B 
}                 // b destroyed -> Outputs: -B 
use(a);           // pass-by-value invokes copy constructor -> Outputs: copyAc 
                  // inside use -> Outputs: use 
                  // temp copy destroyed -> Outputs: -Ac 
Logger* p = new Logger("P"); // Outputs: +P 
delete p;                    // Outputs: -P 
std::cout << "end";          // Outputs: end 
// a destroyed at main exit   -> Outputs: -A 
```
- **Dynamic 2D Allocation**: Implements `seatMap = new char*[rows]` followed by `seatMap[i] = new char[cols]`. Destructor deallocates each row and the row table, ensuring zero memory leaks under RAII.

---

## 5. Q4 — Overloading Arithmetic & Stream Operators (Money Billing)
### "Complete the Code" Implementation
- Overloaded `operator+`: Computes total paise and returns a new `Money` instance.
- Overloaded `operator==`: Compares underlying paise counts.
- Overloaded friend `operator<<`: Prints currency cleanly as `Rs 200.00`.
- **Carry Test Execution**:
  ```
  Rs 0.75 + Rs 0.50 = Rs 1.25
  ```
  Proves paise values >= 100 cleanly roll over into rupees without fractional loss.

---

## 6. Q5 — Increment, Comparison and Subscript [] (Music Playlist)
### Prefix vs Postfix Signature Mechanics
| Operator | Function Signature | Return Type | Internal Mechanics |
|---|---|---|---|
| Prefix `++t` | `TrackTime& operator++()` | Reference (`TrackTime&`) | Increments internal value; returns mutated `*this` (Fast) |
| Postfix `t++` | `TrackTime operator++(int)` | Value (`TrackTime`) | Copies old state to temporary, increments internal value, returns old state by value (Incurs copy overhead) |

- **Subscript `operator[]`**: Implemented both `const` and non-const overloads with bounds verification, preventing buffer overruns.

---

## 7. Q6 — Rules of Operator Overloading (SMS Text Gateway)
### Fundamental Rules of Operator Overloading
1. Precedence and associativity are immutable and cannot be modified.
2. Arity (unary vs binary) cannot be altered.
3. At least one operand must be a user-defined class/struct/enum type.
4. Non-overloadable operators: Scope resolution (`::`), Member access (`.`), Pointer-to-member (`.*`), Ternary conditional (`?:`), `sizeof`, `typeid`.
5. Assignment (`=`), Function call (`()`), Subscript (`[]`), and Member access arrow (`->`) must be non-static member functions.

---

## 8. Q7 — Complex Number Class from First Principles (AC Impedance)
### Mathematical Formulations
- **Rectangular Form**: Z = R + jX, where R is resistance and X is reactance.
- **Magnitude**: |Z| = sqrt(R^2 + X^2).
- **Complex Addition (Series RLC)**:
  Z_series = ZR + ZL + ZC = (50 + 0j) + (0 + 30j) + (0 - 20j) = 50 + 10j Ohms
  |Z_series| = sqrt(50^2 + 10^2) = sqrt(2600) ~ 50.99 Ohms
- **Complex Parallel Combination**:
  Z_parallel = (ZR * ZL) / (ZR + ZL) = (50)(30j) / (50 + 30j) = 13.24 + 22.06j Ohms
  |Z_parallel| = sqrt(13.24^2 + 22.06^2) ~ 25.72 Ohms
- **Zero Division Protection**: Detected dynamically, safely returning zero impedance and printing diagnostic errors rather than causing process termination.
