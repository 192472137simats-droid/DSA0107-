/*
 * Task 03 - Data Types and Type Compatibility
 * Course: Object Oriented Programming with C++
 * Student: G SAI TEJA (192472137)
 *
 * Demonstrates data type selection, sizeof, type compatibility,
 * integer paise vs double rupees, ranges from <climits>, and
 * precision/overflow issues.
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cfloat>
#include <cmath>
#include <string>

using namespace std;

int main() {
    cout << "============================================================" << endl;
    cout << "  TASK 3: Data Types and Type Compatibility                 " << endl;
    cout << "  Student: G SAI TEJA | Reg No: 192472137                   " << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // --- Data Type Table with sizeof ---
    cout << "=== Data Type Selection Table ===" << endl;
    cout << left << setw(22) << "Value" << setw(15) << "Type"
         << setw(10) << "sizeof" << "Justification" << endl;
    cout << string(80, '-') << endl;

    cout << setw(22) << "Zone number"       << setw(15) << "enum class"
         << setw(10) << sizeof(int) << "Type-safe, underlying int" << endl;
    cout << setw(22) << "Fare (paise)"      << setw(15) << "long long"
         << setw(10) << sizeof(long long) << "Exact arithmetic, no rounding" << endl;
    cout << setw(22) << "Balance (paise)"   << setw(15) << "long long"
         << setw(10) << sizeof(long long) << "Same as fare for consistency" << endl;
    cout << setw(22) << "Hour of day"       << setw(15) << "int"
         << setw(10) << sizeof(int) << "0-23, int is sufficient" << endl;
    cout << setw(22) << "Is peak hour"      << setw(15) << "bool"
         << setw(10) << sizeof(bool) << "True/false only" << endl;
    cout << setw(22) << "Journey count"     << setw(15) << "long long"
         << setw(10) << sizeof(long long) << "3M+ journeys, needs 64-bit" << endl;
    cout << setw(22) << "Daily collection"  << setw(15) << "long long"
         << setw(10) << sizeof(long long) << "3M x 3600 = 10.8B paise" << endl;
    cout << setw(22) << "Surcharge %"       << setw(15) << "int"
         << setw(10) << sizeof(int) << "Small integer percentage" << endl;
    cout << setw(22) << "Card ID"           << setw(15) << "string"
         << setw(10) << sizeof(string) << "Variable-length text" << endl;
    cout << setw(22) << "Passenger name"    << setw(15) << "string"
         << setw(10) << sizeof(string) << "Variable-length text" << endl;
    cout << endl;

    // --- Why money in integer paise, not double rupees ---
    cout << "=== Why Integer Paise, Not Double Rupees ===" << endl;

    // Demonstrate floating-point error
    double fare_double = 0.0;
    long long fare_int = 0;

    cout << "Adding Rs 10.00 one hundred times:" << endl;
    for (int i = 0; i < 100; ++i) {
        fare_double += 10.00;
        fare_int += 1000;
    }
    cout << fixed << setprecision(15);
    cout << "  double sum = " << fare_double << " (expected 1000.00)" << endl;
    cout << "  int sum    = " << fare_int << " paise = Rs "
         << fare_int / 100 << "." << setw(2) << setfill('0') << fare_int % 100
         << setfill(' ') << " (exact)" << endl;
    cout << endl;

    // Show the error accumulation
    cout << "Adding Rs 0.10 one thousand times:" << endl;
    double small_sum = 0.0;
    for (int i = 0; i < 1000; ++i) {
        small_sum += 0.10;
    }
    cout << "  double sum = " << small_sum << " (expected 100.00)" << endl;
    cout << "  Error = " << fabs(small_sum - 100.0) << endl;
    cout << endl;
    cout << defaultfloat;

    // --- Type Compatibility Rules ---
    cout << "=== Type Compatibility and Implicit Conversions ===" << endl;

    int intVal = 10;
    double doubleVal = 3.5;
    bool boolVal = true;

    cout << "int + double:  " << intVal << " + " << doubleVal << " = "
         << intVal + doubleVal << " (int promoted to double)" << endl;

    cout << "int + bool:    " << intVal << " + " << boolVal << " = "
         << intVal + boolVal << " (bool promoted to int: true=1)" << endl;

    cout << "double + bool: " << doubleVal << " + " << boolVal << " = "
         << doubleVal + boolVal << " (bool->int->double)" << endl;
    cout << endl;

    // Mixed expression example
    long long farePaise = 1000;
    int surcharge = 20;
    // farePaise * surcharge / 100 — int promoted to long long
    long long result = farePaise * surcharge / 100;
    cout << "farePaise(long long) * surcharge(int) / 100(int) = "
         << result << " (int promoted to long long)" << endl;
    cout << endl;

    // --- Ranges from <climits> ---
    cout << "=== Type Ranges from <climits> and <cfloat> ===" << endl;
    cout << left << setw(20) << "Type" << setw(25) << "Min" << "Max" << endl;
    cout << string(70, '-') << endl;
    cout << setw(20) << "int"       << setw(25) << INT_MIN    << INT_MAX    << endl;
    cout << setw(20) << "unsigned"  << setw(25) << "0"        << UINT_MAX   << endl;
    cout << setw(20) << "long long" << setw(25) << LLONG_MIN  << LLONG_MAX  << endl;
    cout << setw(20) << "double"    << setw(25) << DBL_MIN    << DBL_MAX    << endl;
    cout << endl;

    // --- Can int handle daily collection? ---
    cout << "=== Overflow Analysis: Daily Collection ===" << endl;

    // Worst case: 3,000,000 journeys x Rs 36 (Outer zone peak) = Rs 108,000,000
    // In paise: 3,000,000 x 3600 = 10,800,000,000 paise
    long long maxDailyPaise = 3000000LL * 3600;
    cout << "Max daily collection (3M journeys x Rs 36.00):" << endl;
    cout << "  = " << maxDailyPaise << " paise" << endl;
    cout << "  = Rs " << maxDailyPaise / 100 << endl;
    cout << "  INT_MAX = " << INT_MAX << endl;

    if (maxDailyPaise > INT_MAX) {
        cout << "  *** int OVERFLOWS! " << maxDailyPaise << " > " << INT_MAX << " ***" << endl;
        cout << "  -> long long is REQUIRED for the network total" << endl;
    } else {
        cout << "  int is sufficient." << endl;
    }
    cout << "  LLONG_MAX = " << LLONG_MAX << " — long long is safe" << endl;
    cout << endl;

    // --- Precision comparison: double vs long long ---
    cout << "=== Precision Comparison: n Journey Additions ===" << endl;
    cout << right << setw(12) << "Journeys" << setw(22) << "double (Rs)"
         << setw(22) << "long long (paise)" << setw(15) << "Error (Rs)" << endl;
    cout << string(70, '-') << endl;

    int counts[] = {20000, 500000, 3000000};
    for (int n : counts) {
        double  dTotal = 0.0;
        long long iTotal = 0;
        double fare = 10.00;
        long long farePaiseVal = 1000;

        for (int i = 0; i < n; ++i) {
            dTotal += fare;
            iTotal += farePaiseVal;
        }
        double error = fabs(dTotal - static_cast<double>(iTotal) / 100.0);
        cout << right << setw(12) << n
             << fixed << setprecision(6) << setw(22) << dTotal
             << setw(22) << iTotal
             << setw(15) << error << endl;
    }
    cout << defaultfloat << endl;

    // --- Conclusion ---
    cout << "=== Conclusion ===" << endl;
    cout << "1. Money MUST be stored as long long paise for exact arithmetic." << endl;
    cout << "2. double introduces rounding errors that accumulate over journeys." << endl;
    cout << "3. int overflows at network scale (10.8B paise > INT_MAX 2.1B)." << endl;
    cout << "4. long long (max 9.2 x 10^18) is safe for all foreseeable scales." << endl;
    cout << "5. bool, int are appropriate for flags, hours, and percentages." << endl;

    return 0;
}
