# Task 01 - Polymorphic Hospital Bill Hierarchy (A1 & B1)
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Completes the mandatory "Complete the Code" program:
- `TODO 1`: Declares pure virtual `total() const = 0` and virtual `type() const`.
- `TODO 2`: Implements `OutPatient` overriding `total()` as `consultation + tests` and `type()` as `"OP"`.
- `TODO 3`: Implements `InPatient` overriding `total()` as `days * perDay + surgery` and `type()` as `"IP"`.
Matches the problem specification output:
```
OP Ravi: 1700
IP Meena: 47500
Total 49200
```

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o PolymorphicBilling.exe PolymorphicBilling.cpp
./PolymorphicBilling.exe
```
