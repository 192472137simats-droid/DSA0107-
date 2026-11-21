/**
 * Task12 - Day4DesignChallenge.cpp
 * Day-4 Design Challenge: 4th Zone (Airport), Weekend Discount, Monthly Passes
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

enum class Zone { Inner = 0, Middle = 1, Outer = 2, Airport = 3 };

std::string getZoneName(Zone z) {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
        default:            return "Unknown";
    }
}

class MonthlyPass {
private:
    bool active;
    Zone allowedZone1;
    Zone allowedZone2;

public:
    MonthlyPass(bool isAct, Zone z1, Zone z2)
        : active(isAct), allowedZone1(z1), allowedZone2(z2) {}

    bool covers(Zone z) const {
        if (!active) return false;
        return (z == allowedZone1 || z == allowedZone2);
    }
};

class RedesignedFareEngine {
public:
    static constexpr long long BASE_FARES[] = {1000, 2000, 3000, 6000}; // Airport Rs 60

    static long long calculateFare(Zone z, int hour, bool isWeekend, const MonthlyPass& pass) {
        // Rule: Monthly pass covers unlimited journeys in two chosen zones
        if (pass.covers(z)) {
            return 0; // Free under monthly pass
        }

        long long fare = BASE_FARES[static_cast<int>(z)];
        bool isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);

        if (isWeekend) {
            // Rule: 10% weekend discount that does not combine with peak surcharge
            fare -= (fare * 10) / 100;
        } else if (isPeak) {
            // Weekday peak surcharge: 20%
            fare += (fare * 20) / 100;
        }
        return fare;
    }
};

int main() {
    std::cout << "TASK 12: DAY-4 DESIGN CHALLENGE - ADAPTED METRO ARCHITECTURE\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "============================================================\n\n";

    MonthlyPass commuterPass(true, Zone::Inner, Zone::Middle);

    struct JourneyScenario {
        std::string label;
        Zone zone;
        int hour;
        bool isWeekend;
    };

    std::vector<JourneyScenario> scenarios = {
        {"Airport Journey (Weekday Peak 9 AM)", Zone::Airport, 9, false},
        {"Airport Journey (Weekend 11 AM)", Zone::Airport, 11, true},
        {"Inner Zone Journey with Pass (Weekday Peak)", Zone::Inner, 9, false},
        {"Outer Zone Journey with Inner/Middle Pass (Weekday Off-Peak)", Zone::Outer, 12, false},
        {"Outer Zone Journey on Weekend (18:00, no peak on weekend)", Zone::Outer, 18, true}
    };

    for (const auto& s : scenarios) {
        long long f = RedesignedFareEngine::calculateFare(s.zone, s.hour, s.isWeekend, commuterPass);
        std::cout << "Scenario: " << std::left << std::setw(42) << s.label << "\n"
                  << "  Destination: " << std::setw(8) << getZoneName(s.zone)
                  << "  Time: " << s.hour << ":00"
                  << "  Day: " << (s.isWeekend ? "Weekend" : "Weekday")
                  << " -> Fare: Rs " << std::fixed << std::setprecision(2) << (f / 100.0) << "\n\n";
    }

    return 0;
}
