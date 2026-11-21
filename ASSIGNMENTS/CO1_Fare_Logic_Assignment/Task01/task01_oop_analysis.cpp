/*
 * Task 01 - Understand the Problem and Identify the Objects
 * Course: Object Oriented Programming with C++
 * Student: G SAI TEJA (192472137)
 * 
 * This program demonstrates the objects identified in the Namma Metro
 * Smart-Card Fare System and shows their data and behaviour.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// ============================================================
// OBJECT-ORIENTED DESIGN — Key Objects Identified
// ============================================================

// 1. Zone — represents a metro zone
enum class Zone { Inner, Middle, Outer };

string zoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

// 2. FareTable — encapsulates fare constants and surcharge logic
class FareTable {
    // Data: base fares in paise, surcharge percentage
    static constexpr long long baseFare[] = {1000, 2000, 3000}; // Rs 10, 20, 30
    static constexpr int peakSurchargePercent = 20;
public:
    // Behaviour: compute fare given zone and peak status
    static long long computeFare(Zone z, bool isPeak) {
        long long base = baseFare[static_cast<int>(z)];
        if (isPeak) {
            base += base * peakSurchargePercent / 100;
        }
        return base;
    }

    static void display() {
        cout << "=== Fare Table ===" << endl;
        cout << left << setw(10) << "Zone" << setw(15) << "Off-Peak" << "Peak" << endl;
        cout << string(35, '-') << endl;
        Zone zones[] = {Zone::Inner, Zone::Middle, Zone::Outer};
        for (Zone z : zones) {
            long long offPeak = computeFare(z, false);
            long long peak    = computeFare(z, true);
            cout << left << setw(10) << zoneName(z)
                 << "Rs " << setw(12) << (offPeak / 100.0)
                 << "Rs " << (peak / 100.0) << endl;
        }
    }
};

// 3. SmartCard — represents a passenger's stored-value card
class SmartCard {
    long long balancePaise;    // Data: balance in paise (private — encapsulation)
    string    cardId;          // Data: unique card identifier
    static constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50
public:
    // Constructor — Abstraction: user provides rupees, internally stored as paise
    SmartCard(const string& id, double balanceRupees)
        : balancePaise(static_cast<long long>(balanceRupees * 100)), cardId(id) {}

    // Behaviour: check if travel is possible
    bool canTravel(long long farePaise) const {
        return (balancePaise - farePaise) >= MIN_BALANCE_PAISE;
    }

    // Behaviour: deduct fare (prevents negative effective balance)
    bool deduct(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }

    // Behaviour: recharge card
    void recharge(double rupees) {
        balancePaise += static_cast<long long>(rupees * 100);
    }

    long long getBalancePaise() const { return balancePaise; }
    string    getId()           const { return cardId; }

    void displayBalance() const {
        cout << "Card [" << cardId << "] Balance: Rs "
             << balancePaise / 100 << "."
             << setw(2) << setfill('0') << balancePaise % 100
             << setfill(' ') << endl;
    }
};

// 4. Passenger — represents a passenger with a type (for concessions)
enum class PassengerType { Regular, Student, SeniorCitizen };

class Passenger {
    string        name;
    PassengerType type;
    SmartCard     card;
public:
    Passenger(const string& n, PassengerType t, const string& cardId, double balance)
        : name(n), type(t), card(cardId, balance) {}

    string        getName() const { return name; }
    PassengerType getType() const { return type; }
    SmartCard&    getCard()       { return card; }

    string typeName() const {
        switch (type) {
            case PassengerType::Regular:       return "Regular";
            case PassengerType::Student:       return "Student";
            case PassengerType::SeniorCitizen: return "Senior Citizen";
            default:                           return "Unknown";
        }
    }
};

// 5. Journey — represents a single trip
struct Journey {
    Zone zone;
    bool isPeak;
    long long farePaise;
    bool allowed;
};

// ============================================================
// PROCEDURAL STYLE — for comparison
// ============================================================

namespace Procedural {
    // Global variables — no encapsulation
    long long cardBalance = 0;
    const long long MIN_BAL = 5000;
    const long long fares[] = {1000, 2000, 3000};

    long long computeFare(int zoneIndex, bool peak) {
        long long base = fares[zoneIndex];
        if (peak) base += base * 20 / 100;
        return base;
    }

    bool canTravel(long long fare) {
        return (cardBalance - fare) >= MIN_BAL;
    }

    void deductFare(long long fare) {
        cardBalance -= fare;
    }
}

// ============================================================
// MAIN — Demonstrate both styles and compare
// ============================================================

int main() {
    cout << "============================================================" << endl;
    cout << "  TASK 1: Object-Oriented Analysis of Metro Fare System     " << endl;
    cout << "  Student: G SAI TEJA | Reg No: 192472137                   " << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // --- Display fare table ---
    FareTable::display();
    cout << endl;

    // --- OOP Style Demo ---
    cout << "=== Object-Oriented Style ===" << endl;
    Passenger p("G SAI TEJA", PassengerType::Student, "CARD-001", 150.0);
    cout << "Passenger: " << p.getName() << " (" << p.typeName() << ")" << endl;
    p.getCard().displayBalance();
    cout << endl;

    // Simulate journeys
    Journey trips[] = {
        {Zone::Inner,  false, 0, false},
        {Zone::Outer,  true,  0, false},
        {Zone::Middle, false, 0, false},
        {Zone::Inner,  true,  0, false}
    };

    cout << left << setw(5) << "#" << setw(10) << "Zone" << setw(8) << "Peak"
         << setw(15) << "Fare" << setw(10) << "Status" << "Balance" << endl;
    cout << string(60, '-') << endl;

    int tripNo = 1;
    for (auto& t : trips) {
        t.farePaise = FareTable::computeFare(t.zone, t.isPeak);
        t.allowed = p.getCard().deduct(t.farePaise);
        cout << left << setw(5) << tripNo++
             << setw(10) << zoneName(t.zone)
             << setw(8)  << (t.isPeak ? "Yes" : "No")
             << "Rs " << setw(12) << (t.farePaise / 100.0)
             << setw(10) << (t.allowed ? "OK" : "DENIED")
             << "Rs " << p.getCard().getBalancePaise() / 100.0
             << endl;
    }
    cout << endl;

    // --- Procedural Style Demo ---
    cout << "=== Procedural Style (for comparison) ===" << endl;
    Procedural::cardBalance = 15000; // Rs 150
    cout << "Initial balance: Rs " << Procedural::cardBalance / 100.0 << endl;

    int zones[] = {0, 2, 1, 0};
    bool peaks[] = {false, true, false, true};
    for (int i = 0; i < 4; ++i) {
        long long fare = Procedural::computeFare(zones[i], peaks[i]);
        bool ok = Procedural::canTravel(fare);
        if (ok) Procedural::deductFare(fare);
        cout << "Journey " << i + 1 << ": fare Rs " << fare / 100.0
             << (ok ? " — OK" : " — DENIED")
             << ", balance Rs " << Procedural::cardBalance / 100.0 << endl;
    }
    cout << endl;

    // --- OOP Principles Demonstrated ---
    cout << "=== OOP Principles in This System ===" << endl;
    cout << "1. ENCAPSULATION: SmartCard keeps balancePaise private." << endl;
    cout << "   -> No external code can set a negative balance." << endl;
    cout << "2. ABSTRACTION: User calls deduct(fare), not balancePaise -= fare." << endl;
    cout << "   -> Internal representation (paise) is hidden." << endl;
    cout << "3. INHERITANCE: Student/SeniorCitizen can extend Passenger." << endl;
    cout << "   -> Concession logic varies by passenger type." << endl;
    cout << "4. POLYMORPHISM: computeFare() can be overridden for zones." << endl;
    cout << "   -> New zone types can be added without changing the caller." << endl;
    cout << endl;

    // --- Rules for a Correct Journey ---
    cout << "=== Rules Every Correct Journey Must Satisfy ===" << endl;
    cout << "R1: balance - fare >= Rs 50 (MIN_BALANCE_PAISE = 5000)" << endl;
    cout << "R2: fare = baseFare[zone] + surcharge if peak hours" << endl;
    cout << "R3: Peak hours = 8-10 AM (8 <= h < 10) or 5-8 PM (17 <= h < 20)" << endl;
    cout << "R4: Balance must NEVER go below Rs 50 after any deduction" << endl;
    cout << "R5: All monetary values stored in integer paise (no floating point)" << endl;
    cout << endl;

    // --- Complexity Comparison ---
    cout << "=== Complexity Comparison: Procedural vs OOP ===" << endl;
    cout << "Operation          | Procedural  | OOP (SmartCard)" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Time per journey   | O(1)        | O(1)" << endl;
    cout << "Memory per card    | O(1)        | O(1)" << endl;
    cout << "Change fare rule   | Modify all  | Modify FareTable" << endl;
    cout << "                   | call sites  | class only" << endl;
    cout << "Change min balance | Search all  | Modify SmartCard" << endl;
    cout << "                   | comparisons | constant only" << endl;
    cout << endl;

    cout << "=== Conclusion ===" << endl;
    cout << "OOP design is preferred for the metro fare system because:" << endl;
    cout << "- Encapsulation prevents invalid states (negative balance)." << endl;
    cout << "- Changes to fare rules are localized to one class." << endl;
    cout << "- The system is extensible for new zones and passenger types." << endl;
    cout << "- Time and space complexity remain O(1) per journey in both styles." << endl;

    return 0;
}
