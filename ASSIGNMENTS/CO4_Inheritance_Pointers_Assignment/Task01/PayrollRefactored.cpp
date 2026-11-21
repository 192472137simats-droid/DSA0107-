/**
 * Task01 - PayrollRefactored.cpp
 * Class Design Review & Hierarchy Refactoring (College Payroll)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Refactored Common Base Class
class Staff {
protected:
    std::string name;
    int id;

public:
    Staff(const std::string& n, int i) : name(n), id(i) {}
    virtual ~Staff() = default;

    virtual double pay() const = 0; // Pure virtual function
    virtual std::string getRole() const = 0;

    void printPaySlip() const {
        std::cout << "[" << getRole() << "] ID: " << id 
                  << ", Name: " << std::left << std::setw(16) << name
                  << " -> Net Pay: Rs " << std::fixed << std::setprecision(2) << pay() << "\n";
    }
};

class Professor : public Staff {
private:
    double basic;
    double researchAllowance;
    static constexpr double DA_PERCENT = 0.42;

public:
    Professor(const std::string& n, int i, double b, double r)
        : Staff(n, i), basic(b), researchAllowance(r) {}

    double pay() const override {
        return basic + (basic * DA_PERCENT) + researchAllowance;
    }
    std::string getRole() const override { return "Professor"; }
};

class LabAssistant : public Staff {
private:
    double basic;
    static constexpr double DA_PERCENT = 0.42;

public:
    LabAssistant(const std::string& n, int i, double b)
        : Staff(n, i), basic(b) {}

    double pay() const override {
        return basic + (basic * DA_PERCENT);
    }
    std::string getRole() const override { return "Lab Assistant"; }
};

class GuestFaculty : public Staff {
private:
    double hours;
    double hourlyRate;

public:
    GuestFaculty(const std::string& n, int i, double h, double r)
        : Staff(n, i), hours(h), hourlyRate(r) {}

    double pay() const override {
        return hours * hourlyRate;
    }
    std::string getRole() const override { return "Guest Faculty"; }
};

int main() {
    std::cout << "CO4 - TASK 01: REFACTORED PAYROLL CLASS HIERARCHY\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "=================================================\n\n";

    std::vector<Staff*> collegeStaff = {
        new Professor("Dr. Ramanathan", 101, 85000.0, 15000.0),
        new LabAssistant("Karthik S.", 204, 32000.0),
        new GuestFaculty("Ananya Iyer", 305, 45.0, 1200.0)
    };

    double totalPayroll = 0;
    for (const Staff* s : collegeStaff) {
        s->printPaySlip();
        totalPayroll += s->pay();
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "Total Monthly College Payroll: Rs " 
              << std::fixed << std::setprecision(2) << totalPayroll << "\n";

    for (Staff* s : collegeStaff) delete s;

    return 0;
}
