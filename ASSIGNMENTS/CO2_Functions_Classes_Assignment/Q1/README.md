# Q1 - Call by Value, by Reference and by Pointer
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Implements the Hostel Fee Concession updater across three parameter passing semantics:
1. **Call by value**: operates on a local frame copy; original caller fee unchanged.
2. **Call by reference (`long long&`)**: binds directly to caller's lvalue; changes original in-place.
3. **Call by pointer (`long long*`)**: passes address; dereferencing modifies caller's value.
Includes full execution and verification of the question's Predict the Output and Complete the Code tasks.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o HostelFeeConcession.exe HostelFeeConcession.cpp
./HostelFeeConcession.exe
```
