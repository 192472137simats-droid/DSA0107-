# Q2 - Return by Reference and Its Dangers
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Investigates returning references:
- Returning an lvalue reference allows modifying array elements directly on the left side of `=`.
- Identifies the fatal danger of returning references to stack-allocated local variables (dangling references).
- Explains why container growth in dynamic vectors invalidates previously acquired references.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o MarksRegister.exe MarksRegister.cpp
./MarksRegister.exe
```
