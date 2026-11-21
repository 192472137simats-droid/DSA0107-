# Task 02 - Runtime Binding and Error Handling (A2 & B2)
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Executes both mandatory "Predict the Output" programs analyzing:
1. Virtual table dispatch through non-virtual `show()` and virtual destructor execution order (`~OPBill ~Bill`).
2. Deterministic stack unwinding destroying local `Guard` instances during active exception propagation.
Completes the "Complete the Code" insurance claims audit program demonstrating proper exception validation and `throw;` rethrowing.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o BindingAndExceptions.exe BindingAndExceptions.cpp
./BindingAndExceptions.exe
```
