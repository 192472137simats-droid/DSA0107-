/**
 * Task05 - ShoppingCart.cpp
 * Pointers to Objects, Method Chaining, and the this Pointer
 * Student: G SAI TEJA (192472137)
 */

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // Percent

public:
    // TODO 1: add(): store the item and price if there is space, and return *this so
    //   that calls can be chained
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            n++;
        }
        return *this;
    }

    // TODO 2: setDiscount(): use this-> to tell the member apart from the parameter
    //   named discount, and return *this
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100.0;
    }

    void print() const {
        std::cout << "Cart (" << n << " items, Discount: " << discount << "%): Rs "
                  << std::fixed << std::setprecision(2) << total() << "\n";
    }
};

int main() {
    std::cout << "CO4 - TASK 05: POINTERS TO OBJECTS & THIS POINTER\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "================================================\n\n";

    // 1. Question Complete the Code Verification
    std::cout << "--- 1. Complete the Code Verification ---\n";
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c;
    p->add("Oil", 180);
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';
    // Expected Output: 324.00

    // 2. Finding Highest Total across an Array of Pointers
    std::cout << "\n--- 2. Highest Cart Search using Pointers ---\n";
    Cart c1, c2, c3;
    c1.add("Sugar", 45).add("Tea", 110);
    c2.add("Butter", 80).add("Cheese", 250).setDiscount(5);
    c3.add("Biscuits", 30).add("Milk", 60);

    Cart* activeCarts[3] = { &c1, &c2, &c3 };
    int highestIdx = 0;
    for (int i = 1; i < 3; ++i) {
        if (activeCarts[i]->total() > activeCarts[highestIdx]->total()) {
            highestIdx = i;
        }
    }

    std::cout << "Highest Active Cart is Cart #" << (highestIdx + 1)
              << " with total: Rs " << activeCarts[highestIdx]->total() << "\n";

    return 0;
}
