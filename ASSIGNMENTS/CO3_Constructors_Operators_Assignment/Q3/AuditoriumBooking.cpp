/**
 * Q3 - AuditoriumBooking.cpp
 * Dynamic Constructors, Destructors, and RAII (Auditorium Seat Booking)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>

// --- Predict the Output Program from Question ---
struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};

void use(Logger) { std::cout << "use "; }

void runPredictProgram() {
    std::cout << "--- 1. Predict the Output Verification ---\n";
    Logger a("A");
    {
        Logger b("B");
    }
    use(a);
    Logger* p = new Logger("P");
    delete p;
    std::cout << "end\n";
    // Output: +A +B -B copyAc use -Ac +P -P end -A
}

// --- Dynamic Seat Map Class (RAII) ---
class AuditoriumEvent {
private:
    std::string eventName;
    int rows;
    int seatsPerRow;
    char** seatMap; // 2D dynamic array

public:
    // Dynamic constructor
    AuditoriumEvent(const std::string& name, int r, int s)
        : eventName(name), rows(r), seatsPerRow(s) {
        seatMap = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            seatMap[i] = new char[seatsPerRow];
            for (int j = 0; j < seatsPerRow; ++j) {
                seatMap[i][j] = 'O'; // Open/Available
            }
        }
        std::cout << "[Event Created] \"" << eventName << "\" with " << rows * seatsPerRow << " seats.\n";
    }

    bool bookSeat(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow) return false;
        if (seatMap[r][s] == 'X') return false;
        seatMap[r][s] = 'X'; // Booked
        return true;
    }

    // Destructor (RAII clean-up)
    ~AuditoriumEvent() {
        int booked = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < seatsPerRow; ++j) {
                if (seatMap[i][j] == 'X') booked++;
            }
            delete[] seatMap[i]; // Free row
        }
        delete[] seatMap; // Free row pointer table
        std::cout << "[Event Summary] \"" << eventName << "\" closed. Total Bookings: " 
                  << booked << " / " << rows * seatsPerRow << ". Memory safely freed.\n";
    }
};

int main() {
    std::cout << "CO3 - Q3: DYNAMIC CONSTRUCTORS & DESTRUCTORS (RAII)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "--------------------------------------------------\n\n";

    runPredictProgram();

    std::cout << "\n--- 2. Dynamic 2D Allocation & RAII Simulation ---\n";
    {
        AuditoriumEvent symphony("Grand Annual Symphony", 5, 10);
        symphony.bookSeat(0, 1);
        symphony.bookSeat(0, 2);
        symphony.bookSeat(2, 5);
        symphony.bookSeat(4, 9);
    } // Symphony goes out of scope here; destructor automatically invoked

    return 0;
}
