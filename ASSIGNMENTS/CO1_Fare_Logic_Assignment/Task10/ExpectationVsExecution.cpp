/**
 * Task10 - ExpectationVsExecution.cpp
 * High-Volume Benchmarking & Integer Overflow Forensics
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <climits>

void simulateNetwork(long long journeyCount) {
    std::cout << "--- Simulating " << journeyCount << " Journeys ---\n";
    
    // Average journey fare: Rs 22.00 (2200 paise)
    const long long farePaise = 2200;
    const double fareRupees = 22.0;

    int intPaiseSum = 0;              // 32-bit signed integer (overflow hazard)
    long long llPaiseSum = 0;         // 64-bit integer
    double doubleRupeesSum = 0.0;     // 64-bit floating point

    for (long long i = 0; i < journeyCount; ++i) {
        intPaiseSum += static_cast<int>(farePaise);
        llPaiseSum += farePaise;
        doubleRupeesSum += fareRupees;
    }

    std::cout << "32-bit int accumulator (paise): " << intPaiseSum << " -> Rs " << (intPaiseSum / 100.0) << "\n";
    std::cout << "64-bit long long (paise)       : " << llPaiseSum << " -> Rs " << (llPaiseSum / 100.0) << "\n";
    std::cout << "64-bit double (rupees)         : " << std::fixed << std::setprecision(2) << doubleRupeesSum << " Rs\n";

    if (llPaiseSum != static_cast<long long>(intPaiseSum)) {
        std::cout << ">> ALERT: 32-bit INT OVERFLOW DETECTED! Difference = Rs " 
                  << (llPaiseSum - intPaiseSum) / 100.0 << "\n";
    } else {
        std::cout << ">> 32-bit integer safe for this volume.\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "TASK 10: COMPARISON OF EXPECTATIONS WITH EXECUTION\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "--------------------------------------------------\n\n";

    // 1 Station: 20,000 journeys
    simulateNetwork(20000);

    // 1 Line: 500,000 journeys
    simulateNetwork(500000);

    // Whole Network: 3,000,000 journeys (3M * 2200 paise = 6.6 billion paise > 2.14B INT_MAX)
    simulateNetwork(3000000);

    return 0;
}
