# Q6 - The Rules of Operator Overloading
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Implements custom `Text` class for SMS Gateway:
- Deep copy constructor, destructor, and self-assignment-guarded `operator=`.
- Overloaded `operator+` for string concatenation, `operator==`, `operator!=`, and friend `operator<<`.
- Reviews operator overloading rules (cannot alter precedence, arity, or overload non-overloadable operators like `::`, `.` `.*`, `?:`, `sizeof`).

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o SmsTextGateway.exe SmsTextGateway.cpp
./SmsTextGateway.exe
```
