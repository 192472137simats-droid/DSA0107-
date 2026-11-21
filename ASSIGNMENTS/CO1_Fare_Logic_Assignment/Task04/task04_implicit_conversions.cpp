/*
 * Task 04 - Implicit Conversions
 * Course: Object Oriented Programming with C++
 * Student: G SAI TEJA (192472137)
 *
 * Predict-the-output program + demonstration of implicit conversions,
 * brace initialization, static_cast, and explicit cast usage.
 */

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    cout << "============================================================" << endl;
    cout << "  TASK 4: Control Implicit Conversions                      " << endl;
    cout << "  Student: G SAI TEJA | Reg No: 192472137                   " << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // ============================================================
    // PREDICT THE OUTPUT — Given Program
    // ============================================================
    cout << "=== Predict-the-Output Program ===" << endl;
    cout << endl;

    cout << "--- Predictions (written BEFORE running) ---" << endl;
    cout << "Line 1: 3 3.5"   << endl;
    cout << "Line 2: C 68"    << endl;
    cout << "Line 3: 1 true"  << endl;
    cout << "Line 4: s < u"   << endl;
    cout << endl;

    cout << "--- Actual Output ---" << endl;

    // Line 1: Integer division, then static_cast
    int a = 7, b = 2;
    double c = a / b;                           // 7/2 = 3 (integer division), then 3 -> 3.0
    double d = static_cast<double>(a) / b;      // 7.0/2 = 3.5

    cout << c << ' ' << d << '\n';

    // Line 2: char arithmetic
    char ch = 'A' + 2;                          // 65 + 2 = 67 -> 'C'
    cout << ch << ' ' << ch + 1 << '\n';        // 'C' then 67+1=68 (int promotion)

    // Line 3: int to bool
    bool flag = 5;                              // 5 != 0 -> true (1)
    cout << flag << ' ' << std::boolalpha << flag << '\n';  // 1 true

    // Line 4: signed vs unsigned comparison
    unsigned int u = 1;
    int s = -1;
    cout << (s < u ? "s < u" : "s >= u") << '\n'; // -1 becomes large unsigned!

    cout << endl;

    // ============================================================
    // EXPLANATION OF EACH LINE
    // ============================================================
    cout << "=== Explanation of Each Line ===" << endl;
    cout << endl;

    cout << "LINE 1: " << c << " " << d << endl;
    cout << "  c = a / b = 7 / 2 = 3 (INTEGER DIVISION: both operands are int," << endl;
    cout << "    result is int 3, then implicitly converted to double 3.0)" << endl;
    cout << "  d = static_cast<double>(a) / b = 7.0 / 2 = 3.5" << endl;
    cout << "    (a is explicitly cast to double, b is promoted to double)" << endl;
    cout << endl;

    cout << "LINE 2: " << ch << " " << ch + 1 << endl;
    cout << "  ch = 'A' + 2: 'A' is char (ASCII 65), promoted to int for +" << endl;
    cout << "    65 + 2 = 67, narrowed back to char 'C'" << endl;
    cout << "  ch + 1: ch ('C', 67) is promoted to int, 67 + 1 = 68" << endl;
    cout << "    Result is int, printed as 68 (not 'D')" << endl;
    cout << endl;

    cout << "LINE 3: " << noboolalpha << flag << " " << boolalpha << flag << endl;
    cout << "  flag = 5: int 5 converted to bool (any non-zero -> true)" << endl;
    cout << "  First print: 1 (default bool output)" << endl;
    cout << "  After boolalpha: true (textual representation)" << endl;
    cout << endl;

    cout << noboolalpha;
    cout << "LINE 4: " << (s < u ? "s < u" : "s >= u") << endl;
    cout << "  s = -1 (int), u = 1 (unsigned int)" << endl;
    cout << "  Signed-to-unsigned conversion: -1 becomes " << static_cast<unsigned int>(-1) << endl;
    cout << "  " << static_cast<unsigned int>(-1) << " < 1 is false, so \"s >= u\" is printed" << endl;
    cout << "  ** SURPRISE: -1 appears >= 1 due to unsigned conversion! **" << endl;
    cout << endl;

    // ============================================================
    // CONVERSION POINTS IN FARE SYSTEM
    // ============================================================
    cout << "=== Conversion Points in the Fare System ===" << endl;
    cout << endl;

    // 1. Integer division
    long long farePaise = 1000;
    int surchargePercent = 20;
    long long surcharge_wrong = farePaise * surchargePercent / 100;  // OK: integer math
    double surcharge_double = farePaise * surchargePercent / 100.0;  // implicit int->double

    cout << "1. Integer division: 1000 * 20 / 100 = " << surcharge_wrong
         << " (stays integer)" << endl;
    cout << "   With 100.0: " << surcharge_double
         << " (int promoted to double)" << endl;
    cout << endl;

    // 2. Bool in arithmetic
    bool isPeak = true;
    cout << "2. Bool in arithmetic: isPeak(true) * 200 = " << isPeak * 200
         << " (bool->int: true=1)" << endl;
    cout << endl;

    // 3. Narrowing conversion danger
    cout << "3. Narrowing conversion:" << endl;
    // int x{3.7};  // COMPILE ERROR with brace initialization
    int x_paren = 3.7;    // Warning: narrowing, x_paren = 3
    cout << "   int x = 3.7 -> " << x_paren << " (narrowing, loses .7)" << endl;
    // int safe{3}; // OK — no narrowing
    int safe{3};
    cout << "   int safe{3} -> " << safe << " (brace init, no narrowing)" << endl;
    cout << endl;

    // 4. Enum class requires explicit cast
    enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
    Zone z = Zone::Middle;
    // int idx = z;  // COMPILE ERROR — enum class is NOT implicitly convertible
    int idx = static_cast<int>(z); // Explicit cast needed
    cout << "4. enum class -> int: static_cast<int>(Zone::Middle) = " << idx << endl;
    cout << endl;

    // ============================================================
    // CORRECTED CODE WITH BRACE INIT AND static_cast
    // ============================================================
    cout << "=== Corrected Code with Explicit Conversions ===" << endl;
    cout << endl;

    // Original: double c = a / b;  (implicit integer division)
    // Corrected:
    double c_fixed = static_cast<double>(a) / static_cast<double>(b);
    cout << "double c = static_cast<double>(7) / static_cast<double>(2) = "
         << c_fixed << endl;

    // Original: char ch = 'A' + 2;  (implicit narrowing)
    // Corrected:
    char ch_fixed = static_cast<char>('A' + 2);
    cout << "char ch = static_cast<char>('A' + 2) = " << ch_fixed << endl;

    // Original: bool flag = 5;  (implicit int-to-bool)
    // Corrected:
    bool flag_fixed = (5 != 0);
    cout << "bool flag = (5 != 0) = " << boolalpha << flag_fixed << endl;

    // Original: s < u with signed/unsigned mix
    // Corrected:
    bool cmp_fixed = (s < static_cast<int>(u));
    cout << "s < static_cast<int>(u) = " << cmp_fixed
         << " (compare in signed domain)" << endl;
    cout << endl;

    // ============================================================
    // ALTERNATIVE CONCEPT ANALYSIS
    // ============================================================
    cout << "=== Alternative Concept Analysis ===" << endl;
    cout << "If every conversion were explicit with static_cast:" << endl;
    cout << "- Running time: NO change (casts are zero-cost at runtime)" << endl;
    cout << "- Memory: NO change (same types, same sizes)" << endl;
    cout << "- Compile-time errors caught:" << endl;
    cout << "  * int x{3.7} -> COMPILE ERROR (narrowing detected)" << endl;
    cout << "  * signed/unsigned comparison -> forced explicit handling" << endl;
    cout << "  * enum class -> int requires static_cast" << endl;
    cout << "- Code becomes longer but SAFER and more maintainable" << endl;
    cout << endl;

    cout << "=== Conclusion ===" << endl;
    cout << "1. Integer division truncates — use static_cast<double> when needed" << endl;
    cout << "2. char + int promotes char to int — be explicit about return type" << endl;
    cout << "3. Any non-zero int -> bool true — use explicit comparison" << endl;
    cout << "4. Signed/unsigned comparison can be WRONG — cast to same type" << endl;
    cout << "5. Brace initialization {} prevents narrowing at compile time" << endl;
    cout << "6. static_cast adds zero runtime overhead but catches errors early" << endl;

    return 0;
}
