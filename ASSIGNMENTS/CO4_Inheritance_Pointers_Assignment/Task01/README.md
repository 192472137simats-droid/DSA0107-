# Task 01 - Class Design Review & Hierarchy Refactoring
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Refactors duplicated legacy classes (`Professor`, `LabAssistant`, `GuestFaculty`) into an extensible polymorphic hierarchy under an abstract base class `Staff`.
Eliminates public variables, unifies naming conventions, encapsulates the magic constant `0.42` DA, and enables polymorphic processing in a single unified loop.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o PayrollRefactored.exe PayrollRefactored.cpp
./PayrollRefactored.exe
```
