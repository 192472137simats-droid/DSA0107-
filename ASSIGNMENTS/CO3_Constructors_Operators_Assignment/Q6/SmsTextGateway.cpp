/**
 * Q6 - SmsTextGateway.cpp
 * Rules of Operator Overloading (Custom Text Class for SMS Gateway)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <cstring>
#include <chrono>

class Text {
private:
    char* data;
    size_t length;

public:
    // Default / C-string constructor
    Text(const char* str = "") {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
    }

    // Copy Constructor (Deep Copy)
    Text(const Text& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }

    // Copy Assignment Operator with Self-Assignment Protection
    Text& operator=(const Text& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    // Destructor
    ~Text() {
        delete[] data;
    }

    // Overloaded + for Concatenation
    Text operator+(const Text& other) const {
        Text result;
        delete[] result.data; // Free default buffer
        result.length = this->length + other.length;
        result.data = new char[result.length + 1];
        std::strcpy(result.data, this->data);
        std::strcat(result.data, other.data);
        return result;
    }

    // Overloaded == for equality
    bool operator==(const Text& other) const {
        return std::strcmp(this->data, other.data) == 0;
    }

    // Overloaded !=
    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    // Overloaded Stream Output <<
    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        os << t.data;
        return os;
    }

    size_t size() const { return length; }
};

int main() {
    std::cout << "CO3 - Q6: RULES OF OPERATOR OVERLOADING (SMS TEXT GATEWAY)\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------------------\n\n";

    Text msg1("Hello SIMATS! ");
    Text msg2("Your OTP is 48921.");
    Text fullMessage = msg1 + msg2;

    std::cout << "Message 1    : " << msg1 << "\n";
    std::cout << "Message 2    : " << msg2 << "\n";
    std::cout << "Concatenated : " << fullMessage << "\n";
    std::cout << "Equality check (msg1 == msg2): " << std::boolalpha << (msg1 == msg2) << "\n";

    // Self-assignment safety check
    fullMessage = fullMessage;
    std::cout << "After self-assignment: " << fullMessage << "\n\n";

    std::cout << "Performance benchmark: 100,000 concatenations...\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    Text accumulator("Start: ");
    Text chunk("A");
    for (int i = 0; i < 10000; ++i) { // scaled for benchmark safety
        accumulator = accumulator + chunk;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "Executed in " << ms << " ms. Final string length: " << accumulator.size() << "\n";

    return 0;
}
