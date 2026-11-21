/**
 * Q4 - SavingsAccountMiniStatement.cpp
 * Class Specification with Arrays within a Class (Mini-Statement System)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <string>

class SavingsAccount {
private:
    std::string accountNumber;
    long long balancePaise;
    static const int MAX_TX = 10;
    long long transactions[MAX_TX]; // Circular transaction log buffer
    int txCount;
    int head; // Head pointer for circular indexing

public:
    // Defined inside the class
    SavingsAccount(std::string accNo, long long initialDeposit)
        : accountNumber(accNo), balancePaise(initialDeposit), txCount(0), head(0) {}

    long long getBalance() const {
        return balancePaise;
    }

    // Declared inside, defined outside using scope resolution ::
    void deposit(long long amountPaise);
    bool withdraw(long long amountPaise);
    void miniStatement() const;
};

// Member function definitions outside the class using ::
void SavingsAccount::deposit(long long amountPaise) {
    if (amountPaise <= 0) return;
    balancePaise += amountPaise;
    
    // Store in circular buffer (O(1) insertion)
    transactions[head] = amountPaise; // Positive for deposit
    head = (head + 1) % MAX_TX;
    if (txCount < MAX_TX) txCount++;
}

bool SavingsAccount::withdraw(long long amountPaise) {
    if (amountPaise <= 0 || balancePaise < amountPaise) return false;
    balancePaise -= amountPaise;

    // Store in circular buffer (O(1) insertion)
    transactions[head] = -amountPaise; // Negative for withdrawal
    head = (head + 1) % MAX_TX;
    if (txCount < MAX_TX) txCount++;
    return true;
}

void SavingsAccount::miniStatement() const {
    std::cout << "=========================================\n";
    std::cout << "MINI-STATEMENT FOR A/C: " << accountNumber << "\n";
    std::cout << "Current Balance: Rs " << std::fixed << std::setprecision(2) << (balancePaise / 100.0) << "\n";
    std::cout << "Last " << txCount << " Transactions:\n";
    std::cout << "-----------------------------------------\n";

    int start = (txCount < MAX_TX) ? 0 : head;
    for (int i = 0; i < txCount; ++i) {
        int idx = (start + i) % MAX_TX;
        long long tx = transactions[idx];
        if (tx >= 0) {
            std::cout << "Tx " << std::setw(2) << (i + 1) << ": [CR] +Rs " << std::setw(8) << (tx / 100.0) << "\n";
        } else {
            std::cout << "Tx " << std::setw(2) << (i + 1) << ": [DR] -Rs " << std::setw(8) << (-tx / 100.0) << "\n";
        }
    }
    std::cout << "=========================================\n";
}

int main() {
    std::cout << "CO2 - Q4: CLASS SPECIFICATION WITH ARRAYS (MINI-STATEMENT)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "----------------------------------------------------------\n\n";

    SavingsAccount acc("SB-90214", 500000); // Initial Rs 5,000.00

    // Generate 12 transactions to trigger circular buffer rollover
    acc.deposit(100000);  // 1
    acc.withdraw(50000);  // 2
    acc.deposit(20000);   // 3
    acc.deposit(30000);   // 4
    acc.withdraw(15000);  // 5
    acc.deposit(40000);   // 6
    acc.withdraw(25000);  // 7
    acc.deposit(80000);   // 8
    acc.withdraw(10000);  // 9
    acc.deposit(60000);   // 10
    acc.withdraw(5000);   // 11 (rolls over transaction 1)
    acc.deposit(120000);  // 12 (rolls over transaction 2)

    acc.miniStatement();

    return 0;
}
