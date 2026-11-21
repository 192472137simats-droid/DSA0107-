# SIMATS Engineering Laboratory Assessment Report
## Course: Object Oriented Programming with C++ (CO4)
### Assignment 4: Designing Reusable Class Hierarchies

- **Student Name:** G SAI TEJA
- **Register Number:** 192472137
- **Faculty Reviewer:** Department of Computer Science & Engineering, SIMATS
- **Evaluation Level:** Bloom's Taxonomy Level K5 (Evaluate)
- **Total Marks Assessed:** 50 Marks

---

## 1. Executive Problem Summary
This assessment evaluates the architectural formulation of extensible object-oriented class hierarchies in C++:
1. Diagnosis and refactoring of duplicated classes into polymorphic hierarchies under the Open/Closed Principle.
2. Resolution of the Diamond Problem in multiple inheritance via virtual base classes (`virtual public Person`).
3. Order of construction and destruction in derived and member classes, with member initializer list forwarding.
4. Forensics of base-class pointer operations: static binding of non-virtual methods, memory leakage from missing virtual destructors, and object slicing.
5. Pointers to objects, array-of-pointer searches, and method chaining via the `this` pointer.
6. Day-4 Design Challenge engineering a vehicle rental fleet supporting non-intrusive domain extensions.

---

## 2. Task 01 — Class Design Review & Hierarchy Refactoring
### Diagnosed Deficiencies in Legacy Payroll Code
1. **Redundant Field Definitions**: `name`, `id`, `basic`, and `da` independently declared across all classes.
2. **Encapsulation Breach**: Public data members permit arbitrary external modification.
3. **Inconsistent Naming**: `GuestFaculty` uses `nm` and `code` instead of `name` and `id`.
4. **Dead Data**: The `da` field was stored but never factored into calculations.
5. **Hardcoded Magic Numbers**: The multiplier `0.42` for dearness allowance was repeated across separate functions.
6. **Polymorphic Inability**: Heterogeneous staff could not be processed within a single collection loop.

### Refactored Architecture
Formulated abstract base class `Staff` with pure virtual function `pay() const = 0`. Common fields are protected, and `DA_PERCENT = 0.42` is encapsulated as a `constexpr` constant. Heterogeneous staff are processed in $O(n)$ time via `std::vector<Staff*>`.

---

## 3. Task 02 — University Hierarchy & Virtual Base Classes
### Resolution of the Diamond Problem
When `TeachingAssistant` inherits from both `Student` and `Staff`, both of which inherit from `Person`, two copies of `Person` would be allocated by default, causing ambiguity when referencing `name` or `id`.
Declaring `virtual public Person` in both `Student` and `Staff` collapses the sub-objects into a single shared `Person` instance managed by the most-derived class `TeachingAssistant`.

### Predict the Output Verification
```cpp
TeachingAssistant ta;
// Output: Person Student Staff TA
```
- `Person` is constructed only once because it is a virtual base class.

---

## 4. Task 03 — Order of Construction and Destruction
### Rules of Object Lifecycle
1. **Base Classes**: Initialized in declaration order in the class definition.
2. **Member Objects**: Initialized in declaration order within the class body, regardless of their position in the member initializer list.
3. **Derived Constructor Body**: Executes last.
4. **Destruction**: Occurs in strictly reverse order.

### Predict the Output Verification
```cpp
{ Car c; std::cout << "| "; }
// Output: Vehicle Engine Car | ~Car ~Engine ~Vehicle
```

### Complete the Code Execution Result
```
192311001 Kavya (19) Salem-636001
```
Verified with exact match.

---

## 5. Task 04 — Pointers to Derived Classes Forensics
### Predict the Output Verification
```cpp
SeniorAccount s;
Account* p = &s;
Account copy = s;
std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';
// Output: 3.5 4 3.5
```
- `p->rate()`: Non-virtual call, binds statically at compile time to base rate (`3.5`).
- `p->vrate()`: Virtual call, dynamically dispatches via vtable to derived rate (`4.0`).
- `copy.vrate()`: Value copy triggers **object slicing**, copying only the base sub-object and resetting vtable pointer to `Account`, yielding `3.5`.
- **Destructor Safety**: Declaring `virtual ~Account() = default` ensures `delete p` invokes `SeniorAccount::~SeniorAccount`, releasing dynamic history buffers without memory leaks.

---

## 6. Task 05 — Pointers to Objects & The this Pointer
### Complete the Code Execution Result
- Method chaining via `return *this` allows fluent syntax:
  `c.add("Rice", 60).add("Dal", 120).setDiscount(10);`
- `this->discount = discount` cleanly disambiguates member variables from parameter names.
- Output: `324.00` (Verified).

---

## 7. Task 06 — Day-4 Design Challenge: Vehicle Rental Fleet
- Abstract base class `Vehicle` defines `calculateCharge(days, lateDays)`.
- Derived specializations: `Bicycle`, `MotorVehicle`, `Car`, `ElectricCar`.
- Non-intrusive extension: Added `ElectricBus` with zero modification to existing classes, fully validating the Open/Closed Principle.
