/**
 * Q5 - PlaylistTimer.cpp
 * Overloading ++, Comparison, and Subscript [] (Music Playlist & Timer)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// --- Predict the Output Program from Question ---
struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; } // Prefix
    Counter operator++(int) { Counter old = *this; ++v; return old; } // Postfix
};

void runPredictProgram() {
    std::cout << "--- 1. Predict the Output Verification ---\n";
    Counter c{5};
    Counter a = c++; // Postfix: a takes old (5), c becomes 6
    Counter b = ++c; // Prefix: c becomes 7, b takes 7
    std::cout << a.v << ' ' << b.v << ' ' << c.v << '\n';
    // Output: 5 7 7
}

// --- Music Track Time & Playlist Class ---
class TrackTime {
private:
    int seconds;

public:
    TrackTime(int sec = 0) : seconds(sec) {}

    // Prefix increment (++t)
    TrackTime& operator++() {
        ++seconds;
        return *this;
    }

    // Postfix increment (t++)
    TrackTime operator++(int) {
        TrackTime temp = *this;
        ++seconds;
        return temp;
    }

    bool operator==(const TrackTime& o) const { return seconds == o.seconds; }
    bool operator<(const TrackTime& o) const { return seconds < o.seconds; }

    void print() const {
        int m = seconds / 60;
        int s = seconds % 60;
        std::cout << (m < 10 ? "0" : "") << m << ":" << (s < 10 ? "0" : "") << s;
    }
};

class Playlist {
private:
    std::vector<std::string> tracks;

public:
    void addTrack(const std::string& name) { tracks.push_back(name); }
    size_t size() const { return tracks.size(); }

    // Non-const subscript operator
    std::string& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            std::cerr << "\n[Error] Playlist index out of bounds: " << index << "\n";
            static std::string errorSentinel = "INVALID_INDEX";
            return errorSentinel;
        }
        return tracks[index];
    }

    // Const subscript operator
    const std::string& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            std::cerr << "\n[Error] Playlist index out of bounds: " << index << "\n";
            static const std::string errorSentinel = "INVALID_INDEX";
            return errorSentinel;
        }
        return tracks[index];
    }
};

int main() {
    std::cout << "CO3 - Q5: OVERLOADING ++, COMPARISON, AND [] OPERATORS\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "-----------------------------------------------------\n\n";

    runPredictProgram();

    std::cout << "\n--- 2. Playlist Simulation with Overloaded Operators ---\n";
    Playlist pl;
    pl.addTrack("Acoustic Melody");
    pl.addTrack("Electro Rhythm");
    pl.addTrack("Classical Serenade");

    std::cout << "Track [0]: " << pl[0] << "\n";
    std::cout << "Track [1]: " << pl[1] << "\n";
    std::cout << "Track [2]: " << pl[2] << "\n";

    std::cout << "\nOut-of-range Index Verification (Testing -1 and index 3):\n";
    std::cout << "Access pl[-1]: " << pl[-1] << "\n";
    std::cout << "Access pl[3] : " << pl[3] << "\n";

    std::cout << "\nTimer Increment Verification:\n";
    TrackTime t(59); // 00:59
    std::cout << "Initial Time: "; t.print(); std::cout << "\n";
    ++t; // 01:00 rollover
    std::cout << "After ++t   : "; t.print(); std::cout << "\n";

    return 0;
}
