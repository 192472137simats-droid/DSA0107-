/**
 * Task11 - FareSystemFinalDesign.cpp
 * Recommended Final Architecture: Config-Driven vs Hardcoded
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <map>
#include <string>
#include <chrono>

class MetroConfiguration {
private:
    std::map<std::string, long long> fareMatrix;

public:
    void loadFromVirtualConfig() {
        fareMatrix["Inner"] = 1000;
        fareMatrix["Middle"] = 2000;
        fareMatrix["Outer"] = 3000;
        fareMatrix["Airport"] = 6000;
    }

    long long getFare(const std::string& zone) const {
        auto it = fareMatrix.find(zone);
        if (it != fareMatrix.end()) return it->second;
        return 0;
    }
};

int main() {
    std::cout << "TASK 11: FINAL SYSTEM DESIGN & ARCHITECTURE DEFENCE\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "---------------------------------------------------\n";

    MetroConfiguration config;
    auto t1 = std::chrono::high_resolution_clock::now();
    config.loadFromVirtualConfig();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Loaded dynamic fare configuration in " << duration << " microseconds.\n";
    std::cout << "Inner Zone Fare  : Rs " << config.getFare("Inner") / 100 << "\n";
    std::cout << "Outer Zone Fare  : Rs " << config.getFare("Outer") / 100 << "\n";
    std::cout << "Airport Zone Fare: Rs " << config.getFare("Airport") / 100 << "\n";

    std::cout << "\nArchitecture Evaluation:\n";
    std::cout << "- Separation of business rules from executable code allows instant fare revisions\n";
    std::cout << "  without recompiling and re-deploying gate firmware.\n";
    std::cout << "- Lookup cost (O(1) unordered_map or small array) is negligible compared to gate I/O.\n";

    return 0;
}
