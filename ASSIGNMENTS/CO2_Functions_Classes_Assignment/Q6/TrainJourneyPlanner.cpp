/**
 * Q6 - TrainJourneyPlanner.cpp
 * Friend Functions and Inline Optimization (Train Journey Planner)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <cmath>
#include <chrono>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}

    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

int minutesBetween(const Time& a, const Time& b) {
    // TODO 1: Using the private members h and m, return the absolute difference between
    // the two times in minutes
    int totalA = a.h * 60 + a.m;
    int totalB = b.h * 60 + b.m;
    return std::abs(totalA - totalB);
}

bool earlier(const Time& a, const Time& b) {
    // TODO 2: Return true if a is earlier in the day than b
    if (a.h < b.h) return true;
    if (a.h == b.h && a.m < b.m) return true;
    return false;
}

int main() {
    std::cout << "CO2 - Q6: FRIEND FUNCTIONS & INLINE OPTIMIZATION\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "------------------------------------------------\n\n";

    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' ' 
              << std::boolalpha << earlier(arrival, departure) << '\n';

    // Additional boundary tests
    Time noon1(12, 0), noon2(12, 0);
    std::cout << "Same time difference: " << minutesBetween(noon1, noon2) 
              << ", is earlier: " << earlier(noon1, noon2) << "\n";

    // 10,000,000 call benchmark
    std::cout << "\nBenchmarking 10,000,000 connection planning comparisons...\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    long long checksum = 0;
    for (int i = 0; i < 10000000; ++i) {
        checksum += minutesBetween(departure, arrival);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "Processed 10M operations in " << ms << " ms (Checksum: " << checksum << ")\n";

    return 0;
}
