/**
 * Task08 - SafeInputOutput.cpp
 * Robust Console I/O, Error Recovery, and Formatting
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

void processSimulatedInput(const std::string& inputStr) {
    std::istringstream cin(inputStr);
    int hour = 0;
    long long recharge = 0;

    std::cout << "Simulating input stream: \"" << inputStr << "\"\n";

    std::cout << "Enter hour (0-23): ";
    if (!(cin >> hour) || hour < 0 || hour > 23) {
        std::cout << "INVALID INPUT! Letters or out-of-range detected.\n";
        cin.clear(); // Clear error state
        std::string junk;
        cin >> junk; // Discard bad token
        std::cout << "Recovered! Skipped erroneous token: '" << junk << "'\n";
    } else {
        std::cout << "Accepted Hour: " << hour << ":00\n";
    }

    std::cout << "Enter recharge amount (paise): ";
    if (!(cin >> recharge) || recharge <= 0) {
        std::cout << "REJECTED! Recharge must be positive numeric amount.\n";
    } else {
        std::cout << "Recharged: Rs " << std::fixed << std::setprecision(2) << (recharge / 100.0) << "\n";
    }
}

int main() {
    std::cout << "TASK 08: SAFE INPUT/OUTPUT AND ERROR RECOVERY\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------\n";

    std::cout << "--- Test 1: Invalid alphanumeric hour followed by valid recharge ---\n";
    processSimulatedInput("abc 5000\n");

    std::cout << "\n--- Test 2: Valid inputs ---\n";
    processSimulatedInput("9 2000\n");

    std::cout << "\n--- Test 3: Negative recharge ---\n";
    processSimulatedInput("14 -1000\n");

    return 0;
}
