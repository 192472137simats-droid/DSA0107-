# Task 02 - University People Management & Virtual Base Classes
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Implements the University People Management domain solving the diamond problem:
- `Person` inherited virtually by `Student` and `Staff`.
- `TeachingAssistant` inherits from both `Student` and `Staff`.
- Demonstrates how virtual inheritance guarantees a single shared `Person` sub-object.
- Verifies the mandatory "Predict the Output" program.

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o UniversityHierarchy.exe UniversityHierarchy.cpp
./UniversityHierarchy.exe
```
