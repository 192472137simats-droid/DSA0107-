/*
 * Q3 - Courier Charge Calculator (Main Implementation)
 * Student: G SAI TEJA (192472137)
 * Course: Object Oriented Programming with C++
 * Assignment 2 - CO2: Functions & Classes
 *
 * Demonstrates:
 * 1. Ambiguity between overloaded functions with default arguments
 * 2. Redesigned non-ambiguous overloaded charge functions
 * 3. Overload resolution: exact match, promotion, conversion
 * 4. Default arguments from the right
 *
 * Pseudocode:
 * START
 *   DEFINE struct Parcel with weight, express, insured fields
 *   OVERLOAD charge(int weightGrams) for weight in grams
 *   OVERLOAD charge(double weightKg, bool express=false, bool insured=false)
 *   OVERLOAD charge(const Parcel& p) for Parcel objects
 *   PREDICT which overload each call selects
 *   EXECUTE and verify predictions
 * STOP
 *
 * Time Complexity: O(1) for all charge calculations
 * Space Complexity: O(1) extra per call
 */

#include <iostream>
#include <iomanip>
#include <string>

// --- Parcel structure for the struct-based overload ---
struct Parcel {
    double weightKg;
    bool express;
    bool insured;

    Parcel(double w, bool e = false, bool i = false)
        : weightKg(w), express(e), insured(i) {}
};

// --- Ambiguity Demonstration ---
// The following THREE functions would cause ambiguity:
//   double charge(double weight);
//   double charge(double weight, bool express);
//   double charge(double weight, bool express = false, bool insured = false);
// Because charge(2.0) matches BOTH the first and third (with defaults).
// And charge(2.0, true) matches BOTH the second and third.

// --- Redesigned Non-Ambiguous Overloads ---

// Overload 1: Weight in GRAMS (int parameter)
// Selected by: charge(500), charge(2) [int literal]
double charge(int weightGrams) {
    std::cout << "  [Selected: charge(int weightGrams)] ";
    double weightKg = weightGrams / 1000.0;
    return weightKg * 50.0;  // Rs 50 per kg
}

// Overload 2: Weight in KG (double parameter) with optional express and insured
// Selected by: charge(2.0), charge(1.5, true), charge(2.5, true, true)
// Default arguments: express=false, insured=false
double charge(double weightKg, bool express = false, bool insured = false) {
    std::cout << "  [Selected: charge(double, bool=" << std::boolalpha
              << express << ", bool=" << insured << ")] ";
    double base = weightKg * 50.0;
    if (express) base += 100.0;   // Express surcharge
    if (insured) base += 50.0;    // Insurance surcharge
    return base;
}

// Overload 3: Parcel object
// Selected by: charge(Parcel{...})
double charge(const Parcel& p) {
    std::cout << "  [Selected: charge(const Parcel&)] ";
    double base = p.weightKg * 50.0;
    if (p.express) base += 100.0;
    if (p.insured) base += 50.0;
    return base;
}

int main() {
    std::cout << "=== Q3: Courier Charge Calculator ===\n";
    std::cout << "Student: G SAI TEJA (192472137)\n\n";

    // --- Part 1: Explain the Ambiguity ---
    std::cout << "--- Part 1: Ambiguity Explanation ---\n";
    std::cout << "If we define:\n";
    std::cout << "  double charge(double weight);\n";
    std::cout << "  double charge(double weight, bool express);\n";
    std::cout << "  double charge(double weight, bool express=false, bool insured=false);\n\n";
    std::cout << "Then charge(2.0) is AMBIGUOUS because it matches:\n";
    std::cout << "  - charge(double) [exact match, 1 param]\n";
    std::cout << "  - charge(double, bool=false, bool=false) [exact match + defaults]\n";
    std::cout << "The compiler cannot decide which is better => COMPILE ERROR.\n\n";
    std::cout << "Similarly, charge(2.0, true) matches both the 2-param and 3-param versions.\n\n";

    // --- Part 2: Redesigned Functions ---
    std::cout << "--- Part 2: Redesigned Non-Ambiguous Overloads ---\n";
    std::cout << "Solution: Use DIFFERENT PARAMETER TYPES to disambiguate:\n";
    std::cout << "  charge(int weightGrams)        - for weight in grams\n";
    std::cout << "  charge(double weightKg, ...)    - for weight in kg\n";
    std::cout << "  charge(const Parcel&)           - for Parcel objects\n\n";

    // --- Part 3: Predict and Verify ---
    std::cout << "--- Part 3: Predictions and Verification ---\n\n";

    // Call 1: charge(500) - int literal => selects charge(int)
    std::cout << "Call 1: charge(500)\n";
    std::cout << "  PREDICTION: Selects charge(int weightGrams), weight=500g=0.5kg, result=25.0\n";
    double r1 = charge(500);
    std::cout << "Rs " << r1 << "\n\n";

    // Call 2: charge(2) - int literal => selects charge(int)
    std::cout << "Call 2: charge(2)\n";
    std::cout << "  PREDICTION: Selects charge(int weightGrams), weight=2g=0.002kg, result=0.1\n";
    double r2 = charge(2);
    std::cout << "Rs " << r2 << "\n\n";

    // Call 3: charge(2.0) - double literal => selects charge(double, ...)
    std::cout << "Call 3: charge(2.0)\n";
    std::cout << "  PREDICTION: Selects charge(double), express=false, insured=false, result=100.0\n";
    double r3 = charge(2.0);
    std::cout << "Rs " << r3 << "\n\n";

    // Call 4: charge(2.0f) - float literal => promotion to double => selects charge(double, ...)
    // Note: float->double is a promotion (better than float->int conversion)
    std::cout << "Call 4: charge(2.0f)\n";
    std::cout << "  PREDICTION: float promoted to double => selects charge(double), result=100.0\n";
    double r4 = charge(2.0f);
    std::cout << "Rs " << r4 << "\n\n";

    // Call 5: charge(1.5, true) - double, bool => selects charge(double, bool, bool=false)
    std::cout << "Call 5: charge(1.5, true)\n";
    std::cout << "  PREDICTION: Selects charge(double, bool express=true, bool insured=false)\n";
    std::cout << "              base=75.0 + express=100.0 = 175.0\n";
    double r5 = charge(1.5, true);
    std::cout << "Rs " << r5 << "\n\n";

    // Call 6: charge(Parcel{3.0, true, true})
    std::cout << "Call 6: charge(Parcel{3.0, true, true})\n";
    std::cout << "  PREDICTION: Selects charge(const Parcel&), base=150+100+50=300.0\n";
    double r6 = charge(Parcel{3.0, true, true});
    std::cout << "Rs " << r6 << "\n\n";

    // --- Part 4: Overload Resolution Rules ---
    std::cout << "--- Part 4: Overload Resolution Rules ---\n";
    std::cout << "The compiler chooses among overloaded functions using this priority:\n";
    std::cout << "  1. EXACT MATCH: parameter type matches argument type exactly\n";
    std::cout << "  2. PROMOTION: char/short->int, float->double (widening, lossless)\n";
    std::cout << "  3. STANDARD CONVERSION: int->double, double->int (may lose precision)\n";
    std::cout << "  4. USER-DEFINED CONVERSION: constructors, conversion operators\n\n";
    std::cout << "For charge(2):   int literal => exact match with charge(int)\n";
    std::cout << "For charge(2.0f): float literal => promotion to double => charge(double)\n";
    std::cout << "                  (float->double is promotion, better than float->int conversion)\n\n";

    std::cout << "--- Default Arguments Must Be From the Right ---\n";
    std::cout << "Default arguments must be specified from the rightmost parameter.\n";
    std::cout << "  Valid:   void f(int a, int b = 5, int c = 10);\n";
    std::cout << "  Invalid: void f(int a = 5, int b, int c = 10);  // ERROR\n";
    std::cout << "Reason: Arguments are matched left-to-right; if a middle parameter\n";
    std::cout << "had a default, the compiler couldn't tell if f(1,2) means\n";
    std::cout << "a=1,b=2,c=default or a=default,b=1,c=2.\n\n";

    // --- Part 5: Alternative (Struct-based) ---
    std::cout << "--- Part 5: Alternative - Single Struct Approach ---\n";
    std::cout << "Instead of overloading, use a Parcel struct with optional fields:\n";
    std::cout << "  struct Parcel { double weightKg; bool express=false; bool insured=false; };\n";
    std::cout << "  double charge(const Parcel& p);\n";
    std::cout << "Pros: Adding a new option (e.g., fragile) only adds a field, no new overload.\n";
    std::cout << "Cons: Caller must construct a Parcel object even for simple calls.\n\n";
    std::cout << "Space per call: Parcel by value copies ~12 bytes (double+2 bools),\n";
    std::cout << "  by const reference copies ~8 bytes (pointer). By value int/double\n";
    std::cout << "  copies 4 or 8 bytes.\n";
    std::cout << "Time: Overload resolution is at COMPILE TIME => NO runtime cost.\n";

    return 0;
}
