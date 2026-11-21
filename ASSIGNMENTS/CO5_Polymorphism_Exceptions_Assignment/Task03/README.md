# Task 03 - Abstract Classes & Cost Benchmark (A3 & B3)
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Implements the abstract `Payer` interface with `SelfPay`, `Insurance`, and `GovernmentScheme` derived models.
Executes high-volume performance benchmarking comparing virtual function table indirection against monolithic `switch` statements over 1,000,000 calls.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o PayerBenchmark.exe PayerBenchmark.cpp
./PayerBenchmark.exe
```
