/**
 * Task07 - ProceduralVsOOP.cpp
 * Procedural vs Object-Oriented Fare Logic Implementation
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>

// === PROCEDURAL IMPLEMENTATION ===
namespace Procedural {
    long long globalBalancePaise = 10000; // Exposed global state
    const long long MIN_BALANCE = 5000;

    bool canTravel(long long fare) {
        return (globalBalancePaise - fare) >= MIN_BALANCE;
    }

    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        globalBalancePaise -= fare;
        return true;
    }
}

// === OBJECT-ORIENTED IMPLEMENTATION ===
namespace OOP {
    class SmartCard {
    private:
        long long balancePaise; // Encapsulated private state
        static constexpr long long MIN_BALANCE_PAISE = 5000;

    public:
        explicit SmartCard(long long initialPaise) : balancePaise(initialPaise) {}

        bool canTravel(long long farePaise) const {
            return (balancePaise - farePaise) >= MIN_BALANCE_PAISE;
        }

        bool deduct(long long farePaise) {
            if (!canTravel(farePaise)) return false;
            balancePaise -= farePaise;
            return true;
        }

        long long getBalance() const { return balancePaise; }
    };
}

int main() {
    std::cout << "TASK 07: PROCEDURAL VS OBJECT-ORIENTED DESIGN\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------\n";

    std::cout << "1. Procedural Execution:\n";
    std::cout << "Initial Global Balance: Rs " << Procedural::globalBalancePaise / 100 << "\n";
    Procedural::deduct(2000);
    std::cout << "After Rs 20 deduction : Rs " << Procedural::globalBalancePaise / 100 << "\n";
    // Procedural hazard: Any module can bypass invariants:
    Procedural::globalBalancePaise = -500; // Illegal external corruption!
    std::cout << "Corrupted Balance     : Rs " << Procedural::globalBalancePaise / 100 << " (CRITICAL INVARIANT VIOLATION!)\n\n";

    std::cout << "2. Object-Oriented Execution:\n";
    OOP::SmartCard card(10000);
    std::cout << "Initial Card Balance  : Rs " << card.getBalance() / 100 << "\n";
    card.deduct(2000);
    std::cout << "After Rs 20 deduction : Rs " << card.getBalance() / 100 << "\n";
    bool success = card.deduct(4000); // would leave 4000 < 5000
    std::cout << "Attempt Rs 40 deduct  : " << (success ? "Allowed" : "Denied by Invariant Enforcement") << "\n";
    std::cout << "Final Secure Balance  : Rs " << card.getBalance() / 100 << "\n";

    return 0;
}
