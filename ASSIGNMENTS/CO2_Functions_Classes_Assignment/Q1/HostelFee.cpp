/*
 * Q1 - Hostel Fee Concession Updater (Main Implementation)
 * Student: G SAI TEJA (192472137)
 * Course: Object Oriented Programming with C++
 * Assignment 2 - CO2: Functions & Classes
 *
 * Demonstrates call by value, call by reference, and call by pointer
 * for applying hostel fee concessions.
 *
 * Concessions: Merit 10%, Sports 15%, Single Parent 25%
 *
 * Pseudocode (START/STOP format):
 * START
 *   DECLARE hostelFee = 45000
 *   CALL applyByValue(hostelFee, 10)      // fee unchanged
 *   PRINT hostelFee
 *   CALL applyByReference(hostelFee, 10)   // fee reduced
 *   PRINT hostelFee
 *   CALL applyByPointer(&hostelFee, 15)    // fee reduced
 *   PRINT hostelFee
 * STOP
 *
 * Time Complexity: O(1) for all three versions (single arithmetic operation)
 * Space Complexity:
 *   - By Value: O(1) extra (copy of fee on stack)
 *   - By Reference: O(1) (alias, typically implemented as pointer internally)
 *   - By Pointer: O(1) (pointer variable on stack)
 */

#include <iostream>
#include <iomanip>

// --- Call by Value ---
// Parameter 'fee' is a LOCAL COPY of the caller's variable.
// Stored on the stack frame of this function.
// Modification does NOT affect the caller's variable.
void applyByValue(long long fee, int percent) {
    std::cout << "  [ByValue] fee address inside function: " << &fee << "\n";
    fee -= fee * percent / 100;
    std::cout << "  [ByValue] fee after concession (local): " << fee << "\n";
}

// --- Call by Reference ---
// Parameter 'fee' is an ALIAS for the caller's variable.
// No new object is created; the reference binds to the original.
// Modification DOES affect the caller's variable.
void applyByReference(long long& fee, int percent) {
    std::cout << "  [ByRef] fee address inside function: " << &fee << "\n";
    fee -= fee * percent / 100;
    std::cout << "  [ByRef] fee after concession: " << fee << "\n";
}

// --- Call by Pointer ---
// Parameter 'fee' is a POINTER holding the address of the caller's variable.
// Dereferencing (*fee) accesses the original variable.
// Modification through *fee DOES affect the caller's variable.
void applyByPointer(long long* fee, int percent) {
    std::cout << "  [ByPtr] fee address inside function: " << fee << "\n";
    *fee -= *fee * percent / 100;
    std::cout << "  [ByPtr] fee after concession: " << *fee << "\n";
}

// --- Return-based approach (alternative design) ---
// Returns the new fee instead of modifying in place.
// Use const reference to avoid copying for large objects.
long long applyConcessionReturn(long long fee, int percent) {
    return fee - fee * percent / 100;
}

int main() {
    std::cout << "=== Q1: Hostel Fee Concession Updater ===\n";
    std::cout << "Student: G SAI TEJA (192472137)\n\n";

    long long hostelFee = 45000;
    std::cout << "Initial hostel fee: " << hostelFee << "\n";
    std::cout << "Fee address in main: " << &hostelFee << "\n\n";

    // ---- Test 1: Call by Value (Merit 10%) ----
    std::cout << "--- Test 1: Call by Value (Merit concession 10%) ---\n";
    std::cout << "  Prediction: hostelFee will NOT change (stays 45000)\n";
    applyByValue(hostelFee, 10);
    std::cout << "  hostelFee after call by value: " << hostelFee << "\n";
    std::cout << "  Result: " << (hostelFee == 45000 ? "CORRECT - fee unchanged" : "UNEXPECTED") << "\n\n";

    // ---- Test 2: Call by Reference (Merit 10%) ----
    std::cout << "--- Test 2: Call by Reference (Merit concession 10%) ---\n";
    std::cout << "  Prediction: hostelFee will change to 40500\n";
    applyByReference(hostelFee, 10);
    std::cout << "  hostelFee after call by reference: " << hostelFee << "\n";
    std::cout << "  Result: " << (hostelFee == 40500 ? "CORRECT - fee changed" : "UNEXPECTED") << "\n\n";

    // ---- Test 3: Call by Pointer (Sports 15%) ----
    std::cout << "--- Test 3: Call by Pointer (Sports concession 15%) ---\n";
    std::cout << "  Prediction: hostelFee will change to 34425\n";
    applyByPointer(&hostelFee, 15);
    std::cout << "  hostelFee after call by pointer: " << hostelFee << "\n";
    std::cout << "  Result: " << (hostelFee == 34425 ? "CORRECT - fee changed" : "UNEXPECTED") << "\n\n";

    // ---- Multiple Concessions on one student ----
    std::cout << "--- Multiple Concessions (Merit 10% + Sports 15% + Single Parent 25%) ---\n";
    long long fee2 = 45000;
    std::cout << "  Starting fee: " << fee2 << "\n";
    applyByReference(fee2, 10);  // Merit
    applyByReference(fee2, 15);  // Sports
    applyByReference(fee2, 25);  // Single Parent
    std::cout << "  Final fee after all concessions: " << fee2 << "\n\n";

    // ---- Design Decision: In-place vs Return ----
    std::cout << "--- Design Decision: Return-based approach ---\n";
    long long fee3 = 45000;
    fee3 = applyConcessionReturn(fee3, 10);
    std::cout << "  After return-based merit concession: " << fee3 << "\n";
    fee3 = applyConcessionReturn(fee3, 15);
    std::cout << "  After return-based sports concession: " << fee3 << "\n\n";

    // ---- When to use const reference ----
    std::cout << "--- Const Reference Usage ---\n";
    std::cout << "  Use 'const long long&' when the function should READ but NOT MODIFY the fee.\n";
    std::cout << "  Example: void printFee(const long long& fee) - prevents accidental modification.\n";
    std::cout << "  For large objects (e.g., Student struct), const& avoids expensive copies.\n\n";

    // ---- Summary ----
    std::cout << "=== Summary ===\n";
    std::cout << std::left << std::setw(20) << "Method"
              << std::setw(25) << "Parameter Storage"
              << std::setw(20) << "Modifies Caller?" << "\n";
    std::cout << std::string(65, '-') << "\n";
    std::cout << std::setw(20) << "By Value"
              << std::setw(25) << "Stack (copy)"
              << std::setw(20) << "No" << "\n";
    std::cout << std::setw(20) << "By Reference"
              << std::setw(25) << "Alias (no copy)"
              << std::setw(20) << "Yes" << "\n";
    std::cout << std::setw(20) << "By Pointer"
              << std::setw(25) << "Stack (address copy)"
              << std::setw(20) << "Yes (via deref)" << "\n\n";

    std::cout << "Design Decision: For multiple concessions, the function should CHANGE\n";
    std::cout << "the fee in place (call by reference) because:\n";
    std::cout << "  1. Multiple concessions are applied sequentially to the SAME fee.\n";
    std::cout << "  2. Avoids the caller forgetting to capture the return value.\n";
    std::cout << "  3. Cleaner syntax: applyConcession(fee, 10) vs fee = applyConcession(fee, 10).\n";
    std::cout << "  4. For a Student object, in-place avoids copying a potentially large struct.\n";

    return 0;
}
