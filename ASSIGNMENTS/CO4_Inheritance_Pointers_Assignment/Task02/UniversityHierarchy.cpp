/**
 * Task02 - UniversityHierarchy.cpp
 * University People Management: Diamond Problem & Virtual Base Classes
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>

// --- Predict the Output Program from Question ---
namespace PredictDemo {
    struct Person { Person() { std::cout << "Person "; } };
    struct Student : virtual Person { Student() { std::cout << "Student "; } };
    struct Staff : virtual Person { Staff() { std::cout << "Staff "; } };
    struct TeachingAssistant : Student, Staff {
        TeachingAssistant() { std::cout << "TA "; }
    };

    void run() {
        std::cout << "--- Predict the Output Program Trace ---\n";
        TeachingAssistant ta;
        std::cout << "\n";
        // Output: Person Student Staff TA
    }
}

// --- Full University Domain Hierarchy ---
class Person {
protected:
    std::string name;
    std::string id;

public:
    Person(const std::string& n, const std::string& i) : name(n), id(i) {
        std::cout << "[Constructor] Person (" << name << ")\n";
    }
    virtual ~Person() = default;

    virtual void display() const {
        std::cout << "Person: " << name << ", ID: " << id << "\n";
    }
};

// Virtual inheritance avoids duplicate Person sub-objects in TeachingAssistant
class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;

public:
    Student(const std::string& n, const std::string& i, const std::string& prog, double c)
        : Person(n, i), programme(prog), cgpa(c) {
        std::cout << "[Constructor] Student (" << programme << ")\n";
    }

    void display() const override {
        std::cout << "Student: " << name << ", Prog: " << programme << ", CGPA: " << cgpa << "\n";
    }
};

class Staff : virtual public Person {
protected:
    std::string department;
    double salary;

public:
    Staff(const std::string& n, const std::string& i, const std::string& dept, double sal)
        : Person(n, i), department(dept), salary(sal) {
        std::cout << "[Constructor] Staff (" << department << ")\n";
    }

    void display() const override {
        std::cout << "Staff: " << name << ", Dept: " << department << ", Salary: Rs " << salary << "\n";
    }
};

class TeachingAssistant : public Student, public Staff {
private:
    int hoursPerWeek;

public:
    // In virtual inheritance, the most-derived class must directly initialize the virtual base
    TeachingAssistant(const std::string& n, const std::string& i,
                      const std::string& prog, double c,
                      const std::string& dept, double sal, int hrs)
        : Person(n, i), Student(n, i, prog, c), Staff(n, i, dept, sal), hoursPerWeek(hrs) {
        std::cout << "[Constructor] TeachingAssistant (" << hoursPerWeek << " hrs/week)\n";
    }

    void display() const override {
        std::cout << "TeachingAssistant: " << name << " [ID: " << id << "]\n"
                  << "  Academic: " << programme << " (CGPA: " << cgpa << ")\n"
                  << "  Employment: " << department << " (Salary: Rs " << salary << ")\n"
                  << "  Load: " << hoursPerWeek << " hrs/week\n";
    }
};

int main() {
    std::cout << "CO4 - TASK 02: UNIVERSITY HIERARCHY & VIRTUAL BASE CLASSES\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "=========================================================\n\n";

    PredictDemo::run();

    std::cout << "\n--- Demonstrating Teaching Assistant Construction ---\n";
    TeachingAssistant ta("Sai Teja", "192472137", "M.Tech CSE", 9.4, "CSE Dept", 25000.0, 15);
    std::cout << "\n--- Displaying TA Record ---\n";
    ta.display();

    return 0;
}
