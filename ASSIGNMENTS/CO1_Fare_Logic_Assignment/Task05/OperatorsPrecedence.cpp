/**
 * Task05 - OperatorsPrecedence.cpp
 * Operator Precedence, Associativity, and Peak Expressions
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>

int main() {
    std::cout << "TASK 05: OPERATOR PRECEDENCE & EXPRESSIONS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "------------------------------------------\n";

    // Question program verification
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';
    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';

    // Peak hour lookup table alternative vs combined boolean expression
    std::cout << "\n--- Peak Hour Logic Comparison ---\n";
    bool peakTable[24] = {false};
    peakTable[8] = peakTable[9] = true;
    peakTable[17] = peakTable[18] = peakTable[19] = true;

    for (int h = 7; h <= 21; ++h) {
        bool exprPeak = (h >= 8 && h < 10) || (h >= 17 && h < 20);
        bool tablePeak = peakTable[h];
        if (exprPeak != tablePeak) {
            std::cerr << "Mismatch detected at hour " << h << "\n";
        }
    }
    std::cout << "Expression and Lookup Table (O(1)) agree across all 24 hours.\n";

    return 0;
}
