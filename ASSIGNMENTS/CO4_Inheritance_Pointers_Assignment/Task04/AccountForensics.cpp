/**
 * Task04 - AccountForensics.cpp
 * Pointers to Derived Classes Forensics: Virtual Destructors & Slicing
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>

// --- Predict the Output Program from Question ---
namespace PredictAccount {
    struct Account {
        double rate() const { return 3.5; }
        virtual double vrate() const { return 3.5; }
        virtual ~Account() = default;
    };

    struct SeniorAccount : Account {
        double rate() const { return 4.0; }
        double vrate() const override { return 4.0; }
    };

    void run() {
        std::cout << "--- 1. Predict the Output Verification ---\n";
        SeniorAccount s;
        Account* p = &s;
        Account copy = s; // Object slicing occurs here!
        std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';
        // Output: 3.5 4 3.5
    }
}

// --- Defect Forensics and Repair Demonstration ---
class RepairedAccount {
public:
    virtual double rate() const { return 3.5; }
    virtual ~RepairedAccount() {
        std::cout << "[Destructor] RepairedAccount destroyed safely.\n";
    }
};

class RepairedSeniorAccount : public RepairedAccount {
private:
    double* transactionHistory;

public:
    RepairedSeniorAccount() : transactionHistory(new double[100]) {}
    double rate() const override { return 4.0; }

    ~RepairedSeniorAccount() override {
        delete[] transactionHistory;
        std::cout << "[Destructor] RepairedSeniorAccount memory released cleanly.\n";
    }
};

int main() {
    std::cout << "CO4 - TASK 04: POINTERS TO DERIVED CLASSES FORENSICS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "====================================================\n\n";

    PredictAccount::run();

    std::cout << "\n--- 2. Repaired Polymorphic Deletion (No Leaks) ---\n";
    RepairedAccount* acc = new RepairedSeniorAccount();
    std::cout << "Polymorphic Rate: " << acc->rate() << "%\n";
    delete acc; // Virtual destructor ensures RepairedSeniorAccount::~RepairedSeniorAccount is invoked

    return 0;
}
