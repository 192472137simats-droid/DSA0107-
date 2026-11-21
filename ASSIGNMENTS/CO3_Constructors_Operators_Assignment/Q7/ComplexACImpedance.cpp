/**
 * Q7 - ComplexACImpedance.cpp
 * Class Design from First Principles: Complex Impedance in AC Circuits
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

class Complex {
private:
    double real;
    double imag;

public:
    // Constructors
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }
    double magnitude() const { return std::sqrt(real * real + imag * imag); }

    // Operator Overloading: +
    Complex operator+(const Complex& o) const {
        return Complex(real + o.real, imag + o.imag);
    }

    // Operator Overloading: -
    Complex operator-(const Complex& o) const {
        return Complex(real - o.real, imag - o.imag);
    }

    // Operator Overloading: * -> (a+bi)(c+di) = (ac-bd) + (ad+bc)i
    Complex operator*(const Complex& o) const {
        return Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real);
    }

    // Operator Overloading: / -> (a+bi)/(c+di) = [(ac+bd) + (bc-ad)i] / (c^2+d^2)
    Complex operator/(const Complex& o) const {
        double denom = o.real * o.real + o.imag * o.imag;
        if (denom == 0.0) {
            std::cerr << "\n[Math Error] Division by zero complex impedance!\n";
            return Complex(0, 0);
        }
        return Complex((real * o.real + imag * o.imag) / denom,
                       (imag * o.real - real * o.imag) / denom);
    }

    bool operator==(const Complex& o) const {
        return (std::abs(real - o.real) < 1e-9) && (std::abs(imag - o.imag) < 1e-9);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(2) << c.real;
        if (c.imag >= 0) os << " + " << c.imag << "j";
        else os << " - " << -c.imag << "j";
        return os;
    }
};

int main() {
    std::cout << "CO3 - Q7: COMPLEX IMPEDANCE CALCULATOR (AC CIRCUITS)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "====================================================\n\n";

    // AC Circuit Parameters
    // R = 50 Ohms -> 50 + 0j
    // L = Inductive reactance XL = 30 Ohms -> 0 + 30j
    // C = Capacitive reactance XC = 20 Ohms -> 0 - 20j
    Complex ZR(50.0, 0.0);
    Complex ZL(0.0, 30.0);
    Complex ZC(0.0, -20.0);

    std::cout << "Component Impedances:\n";
    std::cout << "Resistor   ZR : " << ZR << " Ohms\n";
    std::cout << "Inductor   ZL : " << ZL << " Ohms\n";
    std::cout << "Capacitor  ZC : " << ZC << " Ohms\n\n";

    // 1. Series RLC Circuit: Z_series = ZR + ZL + ZC
    Complex Z_series = ZR + ZL + ZC;
    std::cout << "--- Series RLC Circuit Analysis ---\n";
    std::cout << "Total Series Impedance Z_series: " << Z_series << " Ohms\n";
    std::cout << "Magnitude |Z_series|          : " << Z_series.magnitude() << " Ohms\n\n";

    // 2. Parallel RL Circuit: Z_parallel = (ZR * ZL) / (ZR + ZL)
    std::cout << "--- Parallel RL Circuit Analysis ---\n";
    Complex Z_parallel = (ZR * ZL) / (ZR + ZL);
    std::cout << "Total Parallel Impedance Z_par : " << Z_parallel << " Ohms\n";
    std::cout << "Magnitude |Z_parallel|        : " << Z_parallel.magnitude() << " Ohms\n\n";

    // 3. Division by Zero Safety Test
    std::cout << "--- Division by Zero Safety Test ---\n";
    Complex ZeroZ(0.0, 0.0);
    Complex divResult = ZR / ZeroZ;
    std::cout << "Result of ZR / 0: " << divResult << "\n";

    return 0;
}
