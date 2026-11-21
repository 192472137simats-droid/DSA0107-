/**
 * Task09 - FareLogicTestSuite.cpp
 * Complete Fare Logic Implementation & Rigorous Test Suite
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

enum class Zone { Inner, Middle, Outer };

struct TestCase {
    std::string description;
    Zone zone;
    int hour;
    long long initialBalance;
    long long expectedFare;
    bool expectedAllowed;
};

long long computeFare(Zone z, int hour) {
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = 1000; break;
        case Zone::Middle: base = 2000; break;
        case Zone::Outer:  base = 3000; break;
    }
    bool isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    if (isPeak) {
        base += (base * 20) / 100;
    }
    return base;
}

int main() {
    std::cout << "TASK 09: FARE LOGIC TEST SUITE EXECUTION\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "----------------------------------------\n\n";

    std::vector<TestCase> tests = {
        {"Normal Inner Off-Peak (12:00)", Zone::Inner, 12, 10000, 1000, true},
        {"Normal Middle Off-Peak (14:00)", Zone::Middle, 14, 10000, 2000, true},
        {"Normal Outer Off-Peak (11:00)", Zone::Outer, 11, 10000, 3000, true},
        {"Boundary 8:00 AM Morning Peak Start", Zone::Inner, 8, 10000, 1200, true},
        {"Boundary 10:00 AM Morning Peak End (Off-Peak)", Zone::Inner, 10, 10000, 1000, true},
        {"Boundary 17:00 Evening Peak Start", Zone::Outer, 17, 10000, 3600, true},
        {"Boundary 20:00 Evening Peak End (Off-Peak)", Zone::Outer, 20, 10000, 3000, true},
        {"Balance leaving exactly Rs 50", Zone::Inner, 12, 6000, 1000, true}, // 6000 - 1000 = 5000 (Min)
        {"Insufficient balance test", Zone::Outer, 9, 8000, 3600, false}     // 8000 - 3600 = 4400 < 5000
    };

    int passed = 0;
    std::cout << std::left << std::setw(42) << "Test Case"
              << std::setw(12) << "Exp Fare"
              << std::setw(12) << "Act Fare"
              << std::setw(12) << "Status" << "\n";
    std::cout << std::string(78, '-') << "\n";

    for (const auto& t : tests) {
        long long actFare = computeFare(t.zone, t.hour);
        bool actAllowed = (t.initialBalance - actFare) >= 5000;
        bool pass = (actFare == t.expectedFare) && (actAllowed == t.expectedAllowed);
        if (pass) passed++;

        std::cout << std::left << std::setw(42) << t.description
                  << "Rs " << std::setw(9) << (t.expectedFare / 100.0)
                  << "Rs " << std::setw(9) << (actFare / 100.0)
                  << (pass ? "PASSED [OK]" : "FAILED") << "\n";
    }

    std::cout << "\nTest Suite Result: " << passed << " / " << tests.size() << " passed.\n";
    return 0;
}
