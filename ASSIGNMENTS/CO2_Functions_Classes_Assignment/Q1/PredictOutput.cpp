/*
 * Q1 - Predict the Output
 * Student: G SAI TEJA (192472137)
 * Course: Object Oriented Programming with C++
 * Assignment 2 - CO2: Functions & Classes
 *
 * PREDICTION (written BEFORE running):
 * Line 1: change(a, b, &c) where a=1, b=2, c=3
 *   - a is passed by value: local copy becomes 11, original a stays 1
 *   - b is passed by reference: b becomes 12
 *   - c is passed by pointer: *c becomes 13
 *   Output: 1 12 13
 *
 * Line 2: bigger(a, c) returns reference to larger of a(1) and c(13) => c
 *   bigger(a, c) = 100 sets c = 100
 *   Output: 1 100
 *
 * Line 3: show(1)       => x=1, y=5(default), z=10(default) => 16
 *         show(1,2)     => x=1, y=2, z=10(default) => 13
 *         show(1,2,3)   => x=1, y=2, z=3 => 6
 *   Output: 16 13 6 (followed by newline)
 *
 * PREDICTED OUTPUT:
 * 1 12 13
 * 1 100
 * 16 13 6
 */

#include <iostream>

void change(int a, int& b, int* c) {
    a += 10;
    b += 10;
    *c += 10;
}

int& bigger(int& p, int& q) {
    return p > q ? p : q;
}

void show(int x, int y = 5, int z = 10) {
    std::cout << x + y + z << ' ';
}

int main() {
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';

    return 0;
}

/*
 * EXPLANATION (after running):
 * - change(a, b, &c): 'a' passed by value => only local copy modified, caller's a=1 unchanged.
 *   'b' passed by reference => caller's b incremented to 12.
 *   'c' passed by pointer => dereferenced pointer modifies caller's c to 13.
 *
 * - bigger(a, c) = 100: bigger() returns a reference to the larger variable (c=13 > a=1),
 *   so c is assigned 100. a remains 1.
 *
 * - show(1): default y=5, z=10 used => 1+5+10 = 16
 *   show(1,2): default z=10 used => 1+2+10 = 13
 *   show(1,2,3): no defaults used => 1+2+3 = 6
 */
