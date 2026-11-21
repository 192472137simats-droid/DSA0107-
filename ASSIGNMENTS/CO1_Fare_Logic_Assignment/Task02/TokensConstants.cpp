/**
 * Task02 - TokensConstants.cpp
 * Tokens, Keywords, Identifiers, and Constexpr Constants
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>

// Scoped enumeration for zones (strong type safety, no implicit int conversion)
enum class Zone : int {
    Inner = 0,
    Middle = 1,
    Outer = 2
};

// Strongly-typed compile-time constants (constexpr)
namespace FareConstants {
    constexpr long long BASE_FARES_PAISE[] = {1000, 2000, 3000}; // Rs 10, 20, 30
    constexpr int PEAK_SURCHARGE_PERCENT = 20;
    constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00
    
    // Boundary hours for peak windows (24-hour clock)
    constexpr int MORNING_PEAK_START = 8;
    constexpr int MORNING_PEAK_END   = 10;
    constexpr int EVENING_PEAK_START = 17;
    constexpr int EVENING_PEAK_END   = 20;
}

constexpr bool isPeakHour(int hour) {
    return (hour >= FareConstants::MORNING_PEAK_START && hour < FareConstants::MORNING_PEAK_END) ||
           (hour >= FareConstants::EVENING_PEAK_START && hour < FareConstants::EVENING_PEAK_END);
}

int main() {
    std::cout << "TASK 02: TOKENS AND CONSTANTS VERIFICATION\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "-------------------------------------------\n";

    std::cout << "Base Fares (Paise -> Rupees):\n";
    std::cout << "Inner Zone : " << FareConstants::BASE_FARES_PAISE[static_cast<int>(Zone::Inner)]  << " paise (Rs 10.00)\n";
    std::cout << "Middle Zone: " << FareConstants::BASE_FARES_PAISE[static_cast<int>(Zone::Middle)] << " paise (Rs 20.00)\n";
    std::cout << "Outer Zone : " << FareConstants::BASE_FARES_PAISE[static_cast<int>(Zone::Outer)]  << " paise (Rs 30.00)\n";
    
    std::cout << "Peak Surcharge: " << FareConstants::PEAK_SURCHARGE_PERCENT << "%\n";
    std::cout << "Minimum Card Balance Requirement: Rs " << FareConstants::MIN_BALANCE_PAISE / 100 << ".00\n\n";

    std::cout << "Peak Window Boundary Checks:\n";
    int testHours[] = {7, 8, 9, 10, 16, 17, 19, 20, 21};
    for (int h : testHours) {
        std::cout << "Hour " << std::setw(2) << h << ":00 -> " 
                  << (isPeakHour(h) ? "PEAK (Surcharge Applies)" : "OFF-PEAK (Standard Fare)") << "\n";
    }

    return 0;
}
