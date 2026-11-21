/**
 * Q5 - BusTicketSystem.cpp
 * Static Members, Class Invariants, and Arrays of Objects
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>

// --- Predict the Output Program from Question ---
class Token {
    static int next;
    int id;
public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

void runPredictProgram() {
    std::cout << "--- 1. Predict the Output Verification ---\n";
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' 
              << extra.get() << ' ' << Token::issued() << '\n';
    // Output: 101 103 104 4
}

// --- Complete the Code Program from Question ---
class BusTicket {
    static int issued; // Shared by all BusTicket objects
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n) {
        // TODO 1: If fewer than CAPACITY tickets have been issued, increase issued and
        // give this ticket the next seat number; otherwise leave seat as 0 (waiting list)
        if (issued < CAPACITY) {
            seat = ++issued;
        } else {
            seat = 0; // Waiting list
        }
    }

    static int count() {
        // TODO 2: Return the number of seats issued so far
        return issued;
    }

    void print() const {
        std::cout << name << ": " 
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) 
                  << '\n';
    }
};

int BusTicket::issued = 0; // Definition of the static data member

int main() {
    std::cout << "CO2 - Q5: STATIC MEMBERS & ARRAYS OF OBJECTS (BUS TICKETS)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------------------\n\n";

    runPredictProgram();

    std::cout << "\n--- 2. Complete the Code Verification ---\n";
    BusTicket trip[4] = {
        BusTicket("Asha"),
        BusTicket("Bala"),
        BusTicket("Chitra"),
        BusTicket("Dev")
    };

    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << '\n';

    return 0;
}
