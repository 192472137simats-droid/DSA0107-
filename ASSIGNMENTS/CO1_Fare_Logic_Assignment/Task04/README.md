# Task 04 - Control of Implicit Type Conversions
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Executes the mandatory "Predict the Output" program exploring:
1. Integer division truncation vs explicit promotion (`static_cast<double>`).
2. Char arithmetic and implicit promotion to integer during stream insertion.
3. Integer-to-bool coercion.
4. The critical signed/unsigned comparison hazard (`-1` coerced to `4294967295u`).

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o ImplicitConversions.exe ImplicitConversions.cpp
./ImplicitConversions.exe
```
