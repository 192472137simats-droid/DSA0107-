/**
 * Q1 - HostelFeeConcession.cpp
 * Call by Value, by Reference, and by Pointer (Hostel Fee Concession Updater)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>

// Predict the Output Program from Question
void change(int a, int& b, int* c) {
    a += 10;
    b += 10;
    *c += 10;
}

int& bigger(int& p, int& q) {
    return p > q ? p : q;
}

void show(int x, int y = 5, int z = 10) {
    std::cout << x + y + z << ' ';
}

void runPredictProgram() {
    std::cout << "--- 1. Predict the Output Verification ---\n";
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n'; // 1 12 13
    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';             // 1 100
    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';                              // 16 13 6
}

// Complete the Code Program from Question
void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100; // Changes only local copy
}

void applyConcession(long long& fee, int percent) {
    // TODO 1: Reduce the caller's fee by percent using integer arithmetic (call by reference)
    fee -= (fee * percent) / 100;
}

void applyByPointer(long long* fee, int percent) {
    if (fee != nullptr) {
        *fee -= (*fee * percent) / 100;
    }
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "CO2 - Q1: PARAMETER PASSING MODES (HOSTEL FEE SYSTEM)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "====================================================\n\n";

    runPredictProgram();

    std::cout << "\n--- 2. Complete the Code Verification ---\n";
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value: " << hostelFee << '\n';

    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';

    long long sportsFee = 45000;
    applyByPointer(&sportsFee, 15);
    std::cout << "after call by pointer (15% sports): " << sportsFee << '\n';

    return 0;
}
