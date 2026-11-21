/**
 * Task04 - ImplicitConversions.cpp
 * Implicit Type Conversions, Narrowing, and static_cast
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>

int main() {
    std::cout << "TASK 04: IMPLICIT TYPE CONVERSIONS & PREDICTIONS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "------------------------------------------------\n";

    // 1. Integer division vs floating point conversion
    int a = 7, b = 2;
    double c = a / b;                           // Integer division truncated to 3, then converted to 3.0
    double d = static_cast<double>(a) / b;      // Promoted to double division: 3.5
    std::cout << "a / b = " << c << ", static_cast<double>(a) / b = " << d << '\n';

    // 2. Char arithmetic and integer promotion
    char ch = 'A' + 2;                          // 'A' (65) + 2 = 67 ('C')
    std::cout << "ch = " << ch << ", ch + 1 = " << (ch + 1) << '\n'; // ch + 1 promoted to int (68)

    // 3. Int to bool conversion
    bool flag = 5;                              // Non-zero int converts to true (1)
    std::cout << "flag = " << flag << " (" << std::boolalpha << flag << ")\n";

    // 4. Signed vs unsigned comparison hazard
    unsigned int u = 1;
    int s = -1;
    // In s < u, s is implicitly converted to unsigned int (4294967295)
    std::cout << "Comparison (-1 < 1u): " << ((s < u) ? "s < u" : "s >= u") << '\n';

    std::cout << "\nSafe explicit cast resolution:\n";
    if (static_cast<long long>(s) < static_cast<long long>(u)) {
        std::cout << "Explicit cast: s < u (correct arithmetic relationship preserved)\n";
    }

    return 0;
}
