/*
 * Q2 - Alternative: Using std::vector (Rule of Zero)
 * Student: G SAI TEJA | Register Number: 192472137
 *
 * With std::vector<std::string>, the compiler-generated copy ctor
 * already performs a deep copy. No custom copy ctor/assignment/dtor needed.
 *
 * Time to copy n medicines: O(n) — same as manual deep copy
 * Space to copy: O(n) — same
 * Shallow copy would be O(1) time, O(1) extra space, but unsafe.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class PrescriptionVec {
    std::vector<std::string> medicines;

public:
    explicit PrescriptionVec(int count) : medicines(count) {}

    // Rule of Zero: no custom copy ctor, assignment, or destructor needed!

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
    int size() const { return static_cast<int>(medicines.size()); }
};

int main()
{
    std::cout << "=== Alternative: std::vector (Rule of Zero) ===\n\n";

    PrescriptionVec p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    PrescriptionVec q = p;        // compiler-generated deep copy via vector
    q.set(1, "Cetirizine");

    PrescriptionVec r(1);
    r = p;                        // compiler-generated copy assignment
    p.set(0, "Ibuprofen");

    std::cout << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | "
              << r.get(0) << ' ' << r.get(1) << '\n';
    // Expected: Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS

    // ---------- Timing comparison ----------
    std::cout << "\n=== Timing: Copy prescriptions of various sizes ===\n";
    for (int sz : {10, 1000, 100000}) {
        PrescriptionVec big(sz);
        for (int i = 0; i < sz; ++i)
            big.set(i, "Medicine_" + std::to_string(i));

        auto start = std::chrono::high_resolution_clock::now();
        PrescriptionVec copy = big;    // deep copy
        auto end = std::chrono::high_resolution_clock::now();

        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  Copy " << sz << " medicines: " << us << " us\n";
    }

    std::cout << "\n=== Complexity Comparison ===\n";
    std::cout << "Manual deep copy (raw array): O(n) time, O(n) space\n";
    std::cout << "std::vector (Rule of Zero):   O(n) time, O(n) space\n";
    std::cout << "Shallow copy:                 O(1) time, O(1) space — but UNSAFE\n";
    std::cout << "\nConclusion: Use std::vector when possible (Rule of Zero).\n";
    std::cout << "Write custom copy ctor only when managing raw resources.\n";

    return 0;
}
