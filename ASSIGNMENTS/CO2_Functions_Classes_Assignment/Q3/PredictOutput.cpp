/*
 * Q3 - Predict the Output
 * Student: G SAI TEJA (192472137)
 *
 * Note: The predict-the-output for Q3 is embedded within the main CourierCharge.cpp
 * which demonstrates overload resolution predictions.
 * This file shows an additional predict-the-output example for default arguments.
 *
 * PREDICTION (written BEFORE running):
 * charge(500)   => charge(int), 500g = 0.5kg => 0.5*50 = 25.0
 * charge(2.0)   => charge(double), 2kg => 2*50 = 100.0
 * charge(2.0f)  => float promoted to double => charge(double), result = 100.0
 * charge(1.5, true) => charge(double, bool=true, bool=false) => 75+100 = 175.0
 */

#include <iostream>

// Default arguments example
void display(int a, int b = 20, int c = 30) {
    std::cout << a << " " << b << " " << c << "\n";
}

// Overloaded functions
void print(int x) {
    std::cout << "int: " << x << "\n";
}

void print(double x) {
    std::cout << "double: " << x << "\n";
}

void print(const char* x) {
    std::cout << "string: " << x << "\n";
}

/*
 * PREDICTION:
 * display(10)        => a=10, b=20(default), c=30(default) => "10 20 30"
 * display(10, 40)    => a=10, b=40, c=30(default) => "10 40 30"
 * display(10, 40, 50)=> a=10, b=40, c=50 => "10 40 50"
 * print(5)           => exact match int => "int: 5"
 * print(3.14)        => exact match double => "double: 3.14"
 * print("hello")     => exact match const char* => "string: hello"
 */

int main() {
    std::cout << "=== Q3: Predict the Output ===\n\n";

    std::cout << "--- Default Arguments ---\n";
    display(10);
    display(10, 40);
    display(10, 40, 50);

    std::cout << "\n--- Overload Resolution ---\n";
    print(5);
    print(3.14);
    print("hello");

    return 0;
}
