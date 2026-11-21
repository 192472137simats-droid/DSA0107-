# Task 04 - Pointers to Derived Classes: Forensics
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Performs forensics on three critical object-oriented defects:
1. **Non-virtual function call through base pointer**: Binds statically at compile time to base rate (`3.5`).
2. **Missing virtual destructor**: Causes incomplete derived object destruction and memory leakage when deleting through base pointer.
3. **Object Slicing**: Assigning a derived object by value to a base instance slices away derived members and resets vtable pointer to base.
Demonstrates full remediation.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o AccountForensics.exe AccountForensics.cpp
./AccountForensics.exe
```
