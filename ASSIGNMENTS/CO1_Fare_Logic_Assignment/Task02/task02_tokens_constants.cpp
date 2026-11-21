/*
 * Task 02 - Tokens, Keywords, Identifiers and Constants
 * Course: Object Oriented Programming with C++
 * Student: G SAI TEJA (192472137)
 *
 * Demonstrates the five token kinds, constant definition approaches,
 * enum vs enum class, and naming conventions for the metro fare system.
 */

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// ============================================================
// APPROACH 1: #define macros (NOT recommended)
// ============================================================
#define MACRO_INNER_FARE  1000
#define MACRO_MIDDLE_FARE 2000
#define MACRO_OUTER_FARE  3000
#define MACRO_SURCHARGE   20

// ============================================================
// APPROACH 2: const variables
// ============================================================
const int CONST_INNER_FARE  = 1000;
const int CONST_MIDDLE_FARE = 2000;
const int CONST_OUTER_FARE  = 3000;

// ============================================================
// APPROACH 3: constexpr (RECOMMENDED)
// ============================================================
constexpr long long INNER_FARE_PAISE  = 1000;  // Rs 10.00
constexpr long long MIDDLE_FARE_PAISE = 2000;  // Rs 20.00
constexpr long long OUTER_FARE_PAISE  = 3000;  // Rs 30.00
constexpr int       PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;  // Rs 50.00

// ============================================================
// Plain enum (unsafe — values leak into enclosing scope)
// ============================================================
enum PlainZone { PZ_Inner, PZ_Middle, PZ_Outer };

// ============================================================
// enum class (RECOMMENDED — scoped, type-safe)
// ============================================================
enum class Zone : int { Inner = 0, Middle = 1, Outer = 2 };

// Fare table using constexpr array
constexpr long long FARE_TABLE[] = {
    INNER_FARE_PAISE,
    MIDDLE_FARE_PAISE,
    OUTER_FARE_PAISE
};

string zoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

long long computeFare(Zone z, bool isPeak) {
    long long base = FARE_TABLE[static_cast<int>(z)];
    if (isPeak) {
        base += base * PEAK_SURCHARGE_PERCENT / 100;
    }
    return base;
}

int main() {
    cout << "============================================================" << endl;
    cout << "  TASK 2: Tokens, Keywords, Identifiers and Constants       " << endl;
    cout << "  Student: G SAI TEJA | Reg No: 192472137                   " << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // --- Five Kinds of Tokens ---
    cout << "=== Five Kinds of C++ Tokens ===" << endl;
    cout << "1. KEYWORDS:     int, double, const, constexpr, enum, class," << endl;
    cout << "                 switch, case, if, else, return, for, bool" << endl;
    cout << "2. IDENTIFIERS:  INNER_FARE_PAISE, Zone, computeFare, balance" << endl;
    cout << "3. CONSTANTS:    1000, 2000, 3000, 20, 'A', \"Inner\", true" << endl;
    cout << "4. OPERATORS:    +, -, *, /, %, =, ==, !=, +=, <<, >>, &&, ||" << endl;
    cout << "5. PUNCTUATORS:  ;, {, }, (, ), [, ], :, ::" << endl;
    cout << endl;

    // --- Demonstrate constexpr vs #define ---
    cout << "=== Constant Definition Comparison ===" << endl;
    cout << left << setw(20) << "Feature" << setw(15) << "#define"
         << setw(15) << "const" << "constexpr" << endl;
    cout << string(60, '-') << endl;
    cout << setw(20) << "Type checking"   << setw(15) << "No"
         << setw(15) << "Yes" << "Yes" << endl;
    cout << setw(20) << "Scope"           << setw(15) << "Global"
         << setw(15) << "Block/NS" << "Block/NS" << endl;
    cout << setw(20) << "Compile-time"    << setw(15) << "Yes(text)"
         << setw(15) << "Maybe" << "Guaranteed" << endl;
    cout << setw(20) << "Debuggable"      << setw(15) << "No"
         << setw(15) << "Yes" << "Yes" << endl;
    cout << setw(20) << "Memory"          << setw(15) << "0 bytes"
         << setw(15) << "Maybe" << "Usually 0" << endl;
    cout << endl;

    // --- Show #define values (no type safety) ---
    cout << "=== #define macro values ===" << endl;
    cout << "MACRO_INNER_FARE  = " << MACRO_INNER_FARE  << endl;
    cout << "MACRO_MIDDLE_FARE = " << MACRO_MIDDLE_FARE << endl;
    cout << "MACRO_OUTER_FARE  = " << MACRO_OUTER_FARE  << endl;
    cout << "MACRO_SURCHARGE   = " << MACRO_SURCHARGE   << endl;
    // Problem: MACRO_INNER_FARE + 0.5 silently becomes double
    cout << "MACRO_INNER_FARE + 0.5 = " << MACRO_INNER_FARE + 0.5
         << " (implicit double conversion!)" << endl;
    cout << endl;

    // --- Show constexpr values (type-safe) ---
    cout << "=== constexpr values (recommended) ===" << endl;
    cout << "INNER_FARE_PAISE  = " << INNER_FARE_PAISE  << " (long long)" << endl;
    cout << "MIDDLE_FARE_PAISE = " << MIDDLE_FARE_PAISE << " (long long)" << endl;
    cout << "OUTER_FARE_PAISE  = " << OUTER_FARE_PAISE  << " (long long)" << endl;
    cout << "PEAK_SURCHARGE_PERCENT = " << PEAK_SURCHARGE_PERCENT << " (int)" << endl;
    cout << "MIN_BALANCE_PAISE = " << MIN_BALANCE_PAISE << " (long long)" << endl;
    cout << endl;

    // --- enum vs enum class ---
    cout << "=== enum vs enum class ===" << endl;
    // Plain enum: values leak into enclosing scope
    PlainZone pz = PZ_Inner; // No scope qualifier needed
    cout << "Plain enum PZ_Inner = " << pz << " (implicit int conversion)" << endl;
    // This compiles but is dangerous: pz + 10 is valid
    cout << "Plain enum PZ_Inner + 10 = " << pz + 10 << " (no type safety!)" << endl;
    cout << endl;

    // enum class: scoped and type-safe
    Zone z = Zone::Inner; // Scope qualifier required
    cout << "enum class Zone::Inner = " << static_cast<int>(z)
         << " (explicit cast needed)" << endl;
    // Zone::Inner + 10 would NOT compile — type safety enforced
    cout << "Zone::Inner + 10 = COMPILE ERROR (type safety!)" << endl;
    cout << endl;

    // --- Naming Rules ---
    cout << "=== Naming Rules and Conventions ===" << endl;
    cout << "1. UPPER_SNAKE_CASE for constants: INNER_FARE_PAISE" << endl;
    cout << "2. PascalCase for classes/enums: SmartCard, Zone, FareTable" << endl;
    cout << "3. camelCase for functions/variables: computeFare, balancePaise" << endl;
    cout << "4. Must start with letter or underscore, not digit" << endl;
    cout << "5. Cannot use C++ keywords as identifiers" << endl;
    cout << "6. Case-sensitive: Zone != zone != ZONE" << endl;
    cout << endl;

    // --- Fare computation using constexpr constants ---
    cout << "=== Fare Computation with constexpr Constants ===" << endl;
    Zone zones[] = {Zone::Inner, Zone::Middle, Zone::Outer};
    for (Zone zn : zones) {
        long long offPeak = computeFare(zn, false);
        long long peak    = computeFare(zn, true);
        cout << left << setw(10) << zoneName(zn)
             << "Off-peak: Rs " << setw(8) << offPeak / 100.0
             << "Peak: Rs " << peak / 100.0 << endl;
    }
    cout << endl;

    // --- Alternative Analysis: #define vs constexpr ---
    cout << "=== Alternative Concept Analysis ===" << endl;
    cout << "If #define macros were used instead of constexpr:" << endl;
    cout << "- No type checking: #define FARE 1000 is just text substitution" << endl;
    cout << "- No scope: macros pollute the global namespace" << endl;
    cout << "- No debugging: debugger cannot inspect macro values" << endl;
    cout << "- Memory: Both use 0 bytes at runtime (compiled as literals)" << endl;
    cout << "- Running time: No difference — both resolve at compile time" << endl;
    cout << "- Maintenance: Changing a constexpr catches type errors;" << endl;
    cout << "               changing a #define may silently break code" << endl;
    cout << endl;

    cout << "=== Conclusion ===" << endl;
    cout << "constexpr + enum class is the recommended approach because:" << endl;
    cout << "- Type safety prevents accidental mixing of zones and integers" << endl;
    cout << "- Scope isolation avoids naming conflicts" << endl;
    cout << "- Compile-time evaluation ensures zero runtime overhead" << endl;
    cout << "- A fare revision requires changing ONE constexpr value" << endl;

    return 0;
}
