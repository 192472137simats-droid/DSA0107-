/**
 * Q7 - Day4DesignChallenge.cpp
 * Day-4 Design Challenge: Diagnose, Redesign and Validate
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <chrono>
#include <vector>

// Limitation diagnosis: Shifting array requires O(k) copy operations per insertion
class LegacyShiftingBuffer {
    int data[10];
    int count = 0;
public:
    void add(int val) {
        if (count < 10) {
            data[count++] = val;
        } else {
            for (int i = 0; i < 9; ++i) data[i] = data[i + 1]; // O(k) shift!
            data[9] = val;
        }
    }
};

// Redesign: Dynamic circular ring buffer achieving strictly O(1) insertion
template <typename T>
class CircularRingBuffer {
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head;
    size_t count;

public:
    explicit CircularRingBuffer(size_t cap)
        : buffer(cap), capacity(cap), head(0), count(0) {}

    void push(const T& item) {
        buffer[head] = item;
        head = (head + 1) % capacity;
        if (count < capacity) count++;
    }

    size_t size() const { return count; }
};

int main() {
    std::cout << "CO2 - Q7: DAY-4 DESIGN CHALLENGE - DIAGNOSE, REDESIGN & VALIDATE\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "================================================================\n\n";

    const int ITERATIONS = 5000000;

    // 1. Benchmark Legacy Shifting Buffer
    LegacyShiftingBuffer legacy;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) legacy.add(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto msLegacy = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    // 2. Benchmark Circular Ring Buffer Redesign
    CircularRingBuffer<int> ring(10);
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) ring.push(i);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto msRing = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();

    std::cout << "Performance Comparison (" << ITERATIONS << " insertions):\n";
    std::cout << "Legacy Shifting Buffer Time : " << msLegacy << " ms (O(k) element moving)\n";
    std::cout << "Redesigned Circular Ring Time: " << msRing << " ms (O(1) pointer wrapping)\n";
    std::cout << "Performance Speedup Factor  : " << (static_cast<double>(msLegacy) / (msRing > 0 ? msRing : 1)) << "x\n";

    return 0;
}
