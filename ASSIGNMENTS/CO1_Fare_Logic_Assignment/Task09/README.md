# Task 09 - Implementation and Testing
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Validates all fare calculations across a table-driven test harness:
- Normal journeys across all 3 zones.
- Exact boundary transitions (8:00 AM, 10:00 AM, 17:00 PM, 20:00 PM).
- Exact boundary balance edge condition (remaining balance exactly Rs 50.00).

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o FareLogicTestSuite.exe FareLogicTestSuite.cpp
./FareLogicTestSuite.exe
```
