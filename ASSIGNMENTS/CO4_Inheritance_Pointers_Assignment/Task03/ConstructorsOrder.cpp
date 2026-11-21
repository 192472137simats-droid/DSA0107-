/**
 * Task03 - ConstructorsOrder.cpp
 * Order of Construction, Destruction, and Member Initializers
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>

// --- Predict the Output Program from Question ---
namespace PredictTrace {
    struct Engine {
        Engine() { std::cout << "Engine "; }
        ~Engine() { std::cout << "~Engine "; }
    };

    struct Vehicle {
        Vehicle() { std::cout << "Vehicle "; }
        ~Vehicle() { std::cout << "~Vehicle "; }
    };

    struct Car : Vehicle {
        Engine e;
        Car() { std::cout << "Car "; }
        ~Car() { std::cout << "~Car "; }
    };

    void run() {
        std::cout << "--- 1. Predict the Output Trace ---\n";
        {
            Car c;
            std::cout << "| ";
        }
        std::cout << '\n';
        // Output: Vehicle Engine Car | ~Car ~Engine ~Vehicle
    }
}

// --- Complete the Code Program from Question ---
class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address { // Member class (Composition)
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo;
    Address home;

public:
    // TODO 1: Write the constructor: initialise the Person base, regNo and the home
    //   Address member in the initializer list
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(c, p) {}

    void print() const {
        std::cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};

int main() {
    std::cout << "CO4 - TASK 03: CONSTRUCTORS IN DERIVED & MEMBER CLASSES\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "=======================================================\n\n";

    PredictTrace::run();

    std::cout << "\n--- 2. Complete the Code Verification ---\n";
    Student s("Kavya", 19, 192311001, "Salem", 636001);
    s.print();
    // Expected Output: 192311001 Kavya (19) Salem-636001

    return 0;
}
