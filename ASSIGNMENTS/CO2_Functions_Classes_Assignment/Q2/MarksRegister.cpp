/**
 * Q2 - MarksRegister.cpp
 * Return by Reference and Its Hazards (Examination Marks Register)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <vector>

// 1. Safe return by reference to an element of caller-provided array
int& highestMark(int marks[], int size) {
    int maxIdx = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return marks[maxIdx]; // Returns reference to persistent caller array element
}

// 2. Dangerous demonstration: Returning reference to a local variable (Dangling Reference)
// Note: Intentionally documented for academic demonstration
int* dangerousLocalPointer() {
    int localVal = 99;
    return &localVal; // Returning address of stack memory that goes out of scope!
}

int main() {
    std::cout << "CO2 - Q2: RETURN BY REFERENCE AND ITS HAZARDS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------\n\n";

    int marks[5] = {78, 85, 92, 65, 88};

    std::cout << "Original marks: ";
    for (int m : marks) std::cout << m << " ";
    std::cout << "\n";

    // Call on left-hand side of assignment (moderation of +2)
    highestMark(marks, 5) += 2;

    std::cout << "After moderating highest mark (+2): ";
    for (int m : marks) std::cout << m << " ";
    std::cout << "\n";
    std::cout << "New highest mark value: " << highestMark(marks, 5) << "\n\n";

    std::cout << "Vector Reallocation Invalidation Analysis:\n";
    std::vector<int> v = {10, 20, 30};
    int& refFirst = v[0];
    std::cout << "Reference to v[0] before growth: " << refFirst << "\n";
    
    // Forcing multiple reallocations:
    for (int i = 0; i < 1000; ++i) v.push_back(i);
    std::cout << "Notice: Accessing refFirst now is undefined behaviour if memory buffer was reallocated!\n";
    std::cout << "Always prefer indexing or updating references after container growth.\n";

    return 0;
}
