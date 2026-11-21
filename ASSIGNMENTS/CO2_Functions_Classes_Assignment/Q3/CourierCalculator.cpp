/**
 * Q3 - CourierCalculator.cpp
 * Function Overloading and Default Arguments (Courier Charge Calculator)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <string>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
    std::string destination;
};

// Disambiguated function design:
// Overload 1: Integer weight in grams (converts to kg)
double calculateCharge(int weightGrams, bool express = false) {
    double weightKg = weightGrams / 1000.0;
    double base = 50.0 + weightKg * 40.0;
    if (express) base += 75.0;
    return base;
}

// Overload 2: Floating point weight in kilograms with optional express & insurance
double calculateCharge(double weightKg, bool express = false, bool insured = false) {
    double base = 50.0 + weightKg * 40.0;
    if (express) base += 75.0;
    if (insured) base += 50.0;
    return base;
}

// Overload 3: Parcel object overload
double calculateCharge(const Parcel& p) {
    return calculateCharge(p.weightKg, p.express, p.insured);
}

int main() {
    std::cout << "CO2 - Q3: FUNCTION OVERLOADING & DEFAULT ARGUMENTS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "--------------------------------------------------\n\n";

    std::cout << std::fixed << std::setprecision(2);

    // Call 1: int grams exact match -> Overload 1
    std::cout << "Call 1: calculateCharge(500)       -> Rs " << calculateCharge(500) << " (500g, normal)\n";

    // Call 2: int grams + express -> Overload 1
    std::cout << "Call 2: calculateCharge(500, true) -> Rs " << calculateCharge(500, true) << " (500g, express)\n";

    // Call 3: double kg exact match -> Overload 2
    std::cout << "Call 3: calculateCharge(2.5)       -> Rs " << calculateCharge(2.5) << " (2.5kg, normal)\n";

    // Call 4: double kg + express + insured -> Overload 2
    std::cout << "Call 4: calculateCharge(2.5, true, true) -> Rs " << calculateCharge(2.5, true, true) << "\n";

    // Call 5: Parcel structure -> Overload 3
    Parcel p{3.0, true, false, "Metro Hub"};
    std::cout << "Call 5: calculateCharge(Parcel)    -> Rs " << calculateCharge(p) << "\n";

    return 0;
}
