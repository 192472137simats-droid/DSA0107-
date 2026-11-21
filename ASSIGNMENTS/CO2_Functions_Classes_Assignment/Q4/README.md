# Q4 - Specifying a Class with an Array Inside It
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Implements bank savings account mini-statement maintaining the last 10 transactions.
Demonstrates:
- Private data encapsulation preventing unauthorized tampering.
- Circular ring-buffer architecture achieving $O(1)$ constant time insertion without $O(k)$ array shifting overhead.
- Member function definitions both inside class body and outside using scope resolution `::`.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o SavingsAccountMiniStatement.exe SavingsAccountMiniStatement.cpp
./SavingsAccountMiniStatement.exe
```
