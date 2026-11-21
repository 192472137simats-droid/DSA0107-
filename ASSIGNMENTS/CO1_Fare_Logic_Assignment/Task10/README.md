# Task 10 - Compare Expectations with Execution
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Executes growth benchmarks across 20,000 (1 station), 500,000 (1 line), and 3,000,000 (network) journeys.
Formally proves that 32-bit signed integers overflow at the network scale (exceeding 2,147,483,647 paise), proving the strict necessity of 64-bit `long long`.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o ExpectationVsExecution.exe ExpectationVsExecution.cpp
./ExpectationVsExecution.exe
```
