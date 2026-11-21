/**
 * Task01 - FareSystemOOP.cpp
 * Metro Smart-Card Fare System - Object-Oriented Analysis & Model
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <iomanip>

enum class Zone { Inner, Middle, Outer };

std::string zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

class FareTable {
public:
    static long long getBaseFare(Zone z) {
        switch (z) {
            case Zone::Inner:  return 1000; // Rs 10 in paise
            case Zone::Middle: return 2000; // Rs 20 in paise
            case Zone::Outer:  return 3000; // Rs 30 in paise
            default:           return 0;
        }
    }
    
    static long long calculateFare(Zone z, bool isPeak, bool isStudent, bool isSenior) {
        long long fare = getBaseFare(z);
        if (isPeak) {
            fare = fare + (fare * 20) / 100; // 20% peak surcharge
        }
        if (isStudent) {
            fare = fare - (fare * 50) / 100; // 50% student concession
        } else if (isSenior) {
            fare = fare - (fare * 25) / 100; // 25% senior concession
        }
        return fare;
    }
};

class SmartCard {
private:
    std::string cardId;
    long long balancePaise;
    static constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00

public:
    SmartCard(std::string id, long long initialBalancePaise)
        : cardId(id), balancePaise(initialBalancePaise) {}

    bool canTravel(long long farePaise) const {
        return (balancePaise - farePaise) >= MIN_BALANCE_PAISE;
    }

    bool deductFare(long long farePaise) {
        if (!canTravel(farePaise)) {
            return false;
        }
        balancePaise -= farePaise;
        return true;
    }

    void recharge(long long amountPaise) {
        if (amountPaise > 0) {
            balancePaise += amountPaise;
        }
    }

    std::string getId() const { return cardId; }
    long long getBalancePaise() const { return balancePaise; }
    
    void printBalance() const {
        std::cout << "Card [" << cardId << "] Balance: Rs "
                  << balancePaise / 100 << "." 
                  << std::setw(2) << std::setfill('0') << balancePaise % 100 
                  << std::setfill(' ') << "\n";
    }
};

class Passenger {
private:
    std::string name;
    std::string passengerType; // Regular, Student, Senior
    SmartCard card;

public:
    Passenger(std::string n, std::string type, SmartCard c)
        : name(n), passengerType(type), card(c) {}

    bool isStudent() const { return passengerType == "Student"; }
    bool isSenior() const { return passengerType == "Senior"; }
    std::string getName() const { return name; }
    SmartCard& getCard() { return card; }
};

class Journey {
private:
    Zone destinationZone;
    int entryHour;
    bool isPeak;

public:
    Journey(Zone z, int hour) : destinationZone(z), entryHour(hour) {
        isPeak = ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20));
    }

    bool getIsPeak() const { return isPeak; }
    Zone getZone() const { return destinationZone; }
    int getHour() const { return entryHour; }
};

int main() {
    std::cout << "========================================================\n";
    std::cout << "NAMMA METRO SMART-CARD FARE SYSTEM (OOP ARCHITECTURE)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "========================================================\n\n";

    SmartCard card1("SC1001", 10000); // Rs 100.00
    Passenger p1("G Sai Teja", "Student", card1);
    
    p1.getCard().printBalance();

    Journey j1(Zone::Outer, 9); // Peak hour 9 AM, Outer zone
    long long fare1 = FareTable::calculateFare(j1.getZone(), j1.getIsPeak(), p1.isStudent(), p1.isSenior());
    
    std::cout << "Journey: Destination=" << zoneToString(j1.getZone()) 
              << ", Hour=" << j1.getHour() << ":00 (Peak=" << (j1.getIsPeak() ? "Yes" : "No") << ")\n";
    std::cout << "Calculated Concession Fare: Rs " << fare1 / 100 << "." 
              << std::setw(2) << std::setfill('0') << fare1 % 100 << "\n";

    if (p1.getCard().deductFare(fare1)) {
        std::cout << "Status: Journey Approved. Fare Deducted.\n";
    } else {
        std::cout << "Status: Journey Denied (Insufficient Balance after minimum Rs 50 rule).\n";
    }
    p1.getCard().printBalance();

    std::cout << "\nAttempting high-fare journey with insufficient post-balance:\n";
    Journey j2(Zone::Outer, 18); // Peak hour 6 PM
    long long fare2 = FareTable::calculateFare(j2.getZone(), j2.getIsPeak(), false, false);
    std::cout << "Regular Outer Peak Fare: Rs " << fare2 / 100 << "." 
              << std::setw(2) << std::setfill('0') << fare2 % 100 << "\n";

    if (p1.getCard().deductFare(fare2)) {
        std::cout << "Status: Journey Approved.\n";
    } else {
        std::cout << "Status: Denied! Balance would fall below minimum threshold (Rs 50).\n";
    }
    p1.getCard().printBalance();

    return 0;
}
