/**
 * Q2 - PrescriptionRecords.cpp
 * Complete the Code: Deep Copy Constructor & Rule of Three (Hospital Prescription)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>

class Prescription {
    int n;
    std::string* medicines; // Dynamically allocated array

public:
    explicit Prescription(int count) 
        : n(count), medicines(new std::string[count]) {}

    // TODO 1: Copy constructor: allocate a NEW array of the same size and copy every
    // medicine (deep copy)
    Prescription(const Prescription& other) 
        : n(other.n), medicines(new std::string[other.n]) {
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // TODO 2: Copy assignment: guard against self-assignment, build a new array, copy,
    // release the old array and return *this
    Prescription& operator=(const Prescription& other) {
        if (this != &other) {
            std::string* newMedicines = new std::string[other.n];
            for (int i = 0; i < other.n; ++i) {
                newMedicines[i] = other.medicines[i];
            }
            delete[] medicines; // Release old memory
            medicines = newMedicines;
            n = other.n;
        }
        return *this;
    }

    // TODO 3: Release the dynamically allocated array
    ~Prescription() {
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
};

int main() {
    std::cout << "CO3 - Q2: DEEP COPY CONSTRUCTOR & RULE OF THREE\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "----------------------------------------------\n\n";

    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    Prescription q = p; // Copy constructor
    q.set(1, "Cetirizine");

    Prescription r(1);
    r = p; // Copy assignment
    p.set(0, "Ibuprofen");

    std::cout << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | "
              << r.get(0) << ' ' << r.get(1) << '\n';

    std::cout << "\nSelf-assignment test:\n";
    p = p; // Self-assignment guard test
    std::cout << "Self-assignment passed cleanly: " << p.get(0) << " " << p.get(1) << "\n";

    return 0;
}
