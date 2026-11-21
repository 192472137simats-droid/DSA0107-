/*
 * Q1 - Predict the Output: Constructor Order Demo
 * Student: G SAI TEJA | Register Number: 192472137
 *
 * Demonstrates member initialisation order vs list order,
 * and when each constructor runs.
 */

#include <iostream>
#include <string>

class Demo {
    int a;
    int b;    // declared AFTER a
public:
    // Initializer list: b before a — but a is initialised first (declaration order)
    Demo(int x) : b(x), a(b + 1)   // WARNING: a uses b before b is initialised!
    {
        std::cout << "Demo(" << x << "): a=" << a << ", b=" << b << "\n";
    }

    Demo() : Demo(10)
    {
        std::cout << "Default ctor delegated\n";
    }
};

/*
 * PREDICTED OUTPUT (before running):
 * -----------------------------------
 * Demo(10): a=<undefined>, b=10
 * Default ctor delegated
 * Demo(5): a=<undefined>, b=5
 *
 * Explanation:
 * - 'a' is declared before 'b', so a is initialised first.
 * - a = b + 1, but b is NOT yet initialised → undefined behaviour.
 * - Then b = x.
 * - With -Wreorder, g++ warns about this.
 *
 * ACTUAL behaviour: a gets whatever garbage or 0 is in b's memory + 1,
 * then b = x.
 */

int main()
{
    std::cout << "=== Predict the Output: Constructor Order ===\n\n";

    std::cout << "Object d1 (default ctor → delegates to Demo(10)):\n";
    Demo d1;
    std::cout << "\n";

    std::cout << "Object d2 (parameterised Demo(5)):\n";
    Demo d2(5);
    std::cout << "\n";

    return 0;
}
