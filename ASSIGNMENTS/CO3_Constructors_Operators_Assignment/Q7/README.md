# Q7 - Design a Complex-Number Class from First Principles
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Designs from first principles a `Complex` impedance class for AC electrical circuits:
- Rectangular form representation ($R + jX$) and magnitude calculation ($|Z| = \sqrt{R^2 + X^2}$).
- Overloaded operators `+`, `-`, `*`, `/`, `==`, and friend `operator<<`.
- Solves series RLC circuit ($Z_{series} = Z_R + Z_L + Z_C$) and parallel circuit ($Z_{parallel} = rac{Z_1 \cdot Z_2}{Z_1 + Z_2}$).
- Handles division by zero impedance safely.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o ComplexACImpedance.exe ComplexACImpedance.cpp
./ComplexACImpedance.exe
```
