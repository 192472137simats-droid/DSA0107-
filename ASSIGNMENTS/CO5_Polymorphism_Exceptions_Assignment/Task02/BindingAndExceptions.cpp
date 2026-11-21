/**
 * Task02 - BindingAndExceptions.cpp
 * Runtime Binding, Virtual Tables, and Exception Rethrowing (A2 & B2)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <stdexcept>

// --- Predict the Output Program 1 (A2 Virtual Table / Destructor) ---
namespace PredictBinding {
    struct Bill {
        virtual ~Bill() { std::cout << "~Bill "; }
        virtual double total() const = 0;
        void show() const { std::cout << total() << ' '; }
    };

    struct OPBill : Bill {
        ~OPBill() override { std::cout << "~OPBill "; }
        double total() const override { return 500; }
    };

    void run() {
        std::cout << "--- 1. Predict the Output (Binding & Virtual Destructor) ---\n";
        Bill* b = new OPBill;
        b->show();
        delete b;
        std::cout << '\n';
        // Output: 500 ~OPBill ~Bill
    }
}

// --- Predict the Output Program 2 (A5 Stack Unwinding & RAII) ---
namespace PredictStackUnwind {
    struct Guard {
        std::string n;
        explicit Guard(const std::string& s) : n(s) {}
        ~Guard() { std::cout << '~' << n << ' '; }
    };

    void f() {
        Guard g("f");
        throw 42;
    }

    void run() {
        std::cout << "\n--- 2. Predict the Output (Stack Unwinding) ---\n";
        try {
            Guard m("main");
            f();
            std::cout << "not printed ";
        } catch (int x) {
            std::cout << "caught " << x;
        } catch (...) {
            std::cout << "other";
        }
        std::cout << '\n';
        // Output: ~f caught 42 ~main
    }
}

// --- Complete the Code Program (B2 Exception Handling & Rethrow) ---
class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

double approve(double billed, double cover) {
    // TODO 1: Throw std::invalid_argument("negative amount") if billed or cover is
    //   negative; throw ClaimError("exceeds cover") if billed > cover; otherwise return
    //   billed
    if (billed < 0 || cover < 0) {
        throw std::invalid_argument("negative amount");
    }
    if (billed > cover) {
        throw ClaimError("exceeds cover");
    }
    return billed;
}

double settle(double billed, double cover) {
    try {
        return approve(billed, cover);
    } catch (const ClaimError& e) {
        // TODO 2: Print "audit: " followed by the message, then rethrow the SAME
        //   exception
        std::cout << "audit: " << e.what() << '\n';
        throw; // Rethrow the exact same exception object
    }
}

int main() {
    std::cout << "CO5 - TASK 02: RUNTIME BINDING & EXCEPTION STRATEGY\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "===================================================\n\n";

    PredictBinding::run();
    PredictStackUnwind::run();

    std::cout << "\n--- 3. Complete the Code Verification (Insurance Claims) ---\n";
    double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (auto& c : cases) {
        try {
            double paid = settle(c[0], c[1]);
            std::cout << "approved " << paid << '\n';
        } catch (const ClaimError& e) {
            std::cout << "claim rejected: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        }
    }
    // Expected Output:
    // approved 40000
    // audit: exceeds cover
    // claim rejected: exceeds cover
    // error: negative amount

    return 0;
}
