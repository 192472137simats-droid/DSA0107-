/**
 * Task03 - DataTypesAudit.cpp
 * C++ Data Types, System Sizes, and Money Precision Audit
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <climits>
#include <cstdint>
#include <iomanip>

int main() {
    std::cout << "TASK 03: DATA TYPES & SYSTEM SIZES AUDIT\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "----------------------------------------\n";

    std::cout << "sizeof(bool)        : " << sizeof(bool) << " byte(s)\n";
    std::cout << "sizeof(char)        : " << sizeof(char) << " byte(s)\n";
    std::cout << "sizeof(int)         : " << sizeof(int) << " byte(s), max = " << INT_MAX << "\n";
    std::cout << "sizeof(long long)   : " << sizeof(long long) << " byte(s), max = " << LLONG_MAX << "\n";
    std::cout << "sizeof(float)       : " << sizeof(float) << " byte(s)\n";
    std::cout << "sizeof(double)      : " << sizeof(double) << " byte(s)\n";
    std::cout << "sizeof(size_t)      : " << sizeof(size_t) << " byte(s)\n\n";

    // Precision Drift Demonstration: Double vs Integer Paise
    std::cout << "--- Precision Drift Audit (100,000 Journeys @ Rs 10.20) ---\n";
    double doubleTotalRupees = 0.0;
    long long paiseTotal = 0;

    const double tripCostRupees = 10.20;
    const long long tripCostPaise = 1020; // 10.20 * 100

    for (int i = 0; i < 100000; ++i) {
        doubleTotalRupees += tripCostRupees;
        paiseTotal += tripCostPaise;
    }

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Accumulated with double  : " << doubleTotalRupees << " Rupees\n";
    std::cout << "Accumulated with int-paise: " << paiseTotal / 100.0 << " Rupees (" << paiseTotal << " paise)\n";
    std::cout << "Discrepancy (Floating error): " << (doubleTotalRupees - (paiseTotal / 100.0)) << " Rupees\n";

    std::cout << "\nConclusion: Storing currency in integer paise eliminates floating point rounding errors.\n";
    return 0;
}
