/**
 * Task04 - HospitalSystemFinal.cpp
 * Extensible Architecture & Production Decision (A4, A5, B4)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <stdexcept>

// Base Exception Hierarchy
class HospitalException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidBillDataException : public HospitalException {
public:
    using HospitalException::HospitalException;
};

// Abstract Bill
class Bill {
protected:
    std::string patientId;

public:
    explicit Bill(const std::string& pid) : patientId(pid) {}
    virtual ~Bill() = default;

    virtual double calculateTotal() const = 0;
    virtual std::string getBillCategory() const = 0;
    std::string getPatientId() const { return patientId; }
};

// Existing types
class ConsultationBill : public Bill {
    double fee;
public:
    ConsultationBill(const std::string& pid, double f) : Bill(pid), fee(f) {
        if (f < 0) throw InvalidBillDataException("Consultation fee cannot be negative");
    }
    double calculateTotal() const override { return fee; }
    std::string getBillCategory() const override { return "Consultation"; }
};

// Seamless Demonstration of Extension: Day-Care Surgery & Corporate Payer
class DayCareSurgeryBill : public Bill {
private:
    double packageRate;
    double consumables;

public:
    DayCareSurgeryBill(const std::string& pid, double pkg, double cons)
        : Bill(pid), packageRate(pkg), consumables(cons) {
        if (pkg < 0 || cons < 0) throw InvalidBillDataException("Surgery charges must be positive");
    }

    double calculateTotal() const override {
        return packageRate + consumables;
    }
    std::string getBillCategory() const override {
        return "Day-Care Surgery (Extension)";
    }
};

int main() {
    std::cout << "CO5 - TASK 04: EXTENSIBLE HOSPITAL SYSTEM & PRODUCTION DEFENSE\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "==============================================================\n\n";

    std::vector<std::unique_ptr<Bill>> batch;
    
    // Add existing and newly extended bill types
    batch.push_back(std::make_unique<ConsultationBill>("PAT-001", 1500.0));
    batch.push_back(std::make_unique<DayCareSurgeryBill>("PAT-002", 28000.0, 4500.0)); // Extension!

    double grandTotal = 0;
    for (const auto& b : batch) {
        std::cout << "Patient: " << b->getPatientId()
                  << " | Category: " << std::left << std::setw(30) << b->getBillCategory()
                  << " | Total: Rs " << std::fixed << std::setprecision(2) << b->calculateTotal() << "\n";
        grandTotal += b->calculateTotal();
    }

    std::cout << "--------------------------------------------------------------\n";
    std::cout << "Batch Total: Rs " << grandTotal << "\n\n";

    // Exception safety test
    std::cout << "Testing Exception Safety with Negative Input:\n";
    try {
        ConsultationBill invalid("PAT-ERR", -500.0);
    } catch (const HospitalException& e) {
        std::cout << "[Caught Exception] Successfully caught: " << e.what() << "\n";
        std::cout << "System remains operational with zero resource leakage.\n";
    }

    return 0;
}
