/**
 * Q4 - MoneyBilling.cpp
 * Complete the Code: Overloading Arithmetic and Stream Operators (Money Class)
 * Student: G SAI TEJA (192472137)
 */

#include <iomanip>
#include <iostream>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    // TODO 1: Overload + as a member function that returns a new Money object
    Money operator+(const Money& other) const {
        Money res;
        res.paise = this->paise + other.paise;
        return res;
    }

    // TODO 2: Overload == as a member, and << as a friend that prints "Rs 200.00"
    bool operator==(const Money& other) const {
        return this->paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        os << "Rs " << (m.paise / 100) << '.' 
           << std::setw(2) << std::setfill('0') << (m.paise % 100)
           << std::setfill(' ');
        return os;
    }

    Money operator-(const Money& other) const {
        Money res;
        res.paise = this->paise - other.paise;
        return res;
    }

    Money& operator+=(const Money& other) {
        this->paise += other.paise;
        return *this;
    }

    bool operator<(const Money& other) const {
        return this->paise < other.paise;
    }
};

int main() {
    std::cout << "CO3 - Q4: OVERLOADING ARITHMETIC & STREAM OPERATORS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "--------------------------------------------------\n\n";

    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';

    // Additional Carry Test: Rs 0.75 + Rs 0.50 = Rs 1.25
    std::cout << "\nAdditional Carry Test (Paise carrying to Rupees):\n";
    Money p1(0, 75);
    Money p2(0, 50);
    Money pSum = p1 + p2;
    std::cout << p1 << " + " << p2 << " = " << pSum << "\n";

    return 0;
}
