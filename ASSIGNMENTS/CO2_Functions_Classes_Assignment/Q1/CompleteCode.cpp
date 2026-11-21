/*
 * Q1 - Complete the Code
 * Student: G SAI TEJA (192472137)
 * Course: Object Oriented Programming with C++
 * Assignment 2 - CO2: Functions & Classes
 *
 * TODO 1 completed: Reduce the caller's fee by percent using call by reference.
 *
 * Expected output:
 * after call by value: 45000
 * after call by reference: 40500
 */

#include <iostream>

void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100;
    // changes only the local copy
}

void applyConcession(long long& fee, int percent) {
    // TODO 1: Reduce the caller's fee by percent using integer arithmetic (call by reference)
    fee -= fee * percent / 100;
}

int main() {
    long long hostelFee = 45000;

    applyByValue(hostelFee, 10);
    std::cout << "after call by value: " << hostelFee << '\n';

    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';

    return 0;
}

/*
 * EXPLANATION:
 * applyByValue receives a COPY of hostelFee. The local copy is reduced to 40500,
 * but the caller's hostelFee remains 45000 because changes to a copy don't affect the original.
 *
 * applyConcession receives a REFERENCE to hostelFee. The reference is an alias
 * for the original variable, so fee -= fee * percent / 100 directly modifies
 * hostelFee from 45000 to 40500.
 */
