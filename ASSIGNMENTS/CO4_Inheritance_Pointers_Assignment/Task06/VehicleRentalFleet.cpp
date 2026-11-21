/**
 * Task06 - VehicleRentalFleet.cpp
 * Day-4 Design Challenge: Extensible Vehicle Rental Fleet Hierarchy
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Abstract Base Class
class Vehicle {
protected:
    std::string regNumber;
    double dailyRate;
    bool isRented;

public:
    Vehicle(const std::string& reg, double rate)
        : regNumber(reg), dailyRate(rate), isRented(false) {}
    virtual ~Vehicle() = default;

    virtual double calculateCharge(int days, int lateDays) const = 0;
    virtual std::string getType() const = 0;

    void rentVehicle() { isRented = true; }
    void returnVehicle() { isRented = false; }
    std::string getReg() const { return regNumber; }
};

// 1. Non-motorized Vehicle
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, double rate) : Vehicle(reg, rate) {}

    double calculateCharge(int days, int lateDays) const override {
        double standard = days * dailyRate;
        double latePenalty = lateDays * (dailyRate * 1.5);
        return standard + latePenalty;
    }
    std::string getType() const override { return "Bicycle"; }
};

// 2. Motorized Vehicle Base
class MotorVehicle : public Vehicle {
protected:
    double fuelPercent;
    std::string serviceDueDate;

public:
    MotorVehicle(const std::string& reg, double rate, double fuel, const std::string& date)
        : Vehicle(reg, rate), fuelPercent(fuel), serviceDueDate(date) {}
};

// 3. Passenger Car
class Car : public MotorVehicle {
private:
    int seatingCapacity;

public:
    Car(const std::string& reg, double rate, double fuel, const std::string& date, int seats)
        : MotorVehicle(reg, rate, fuel, date), seatingCapacity(seats) {}

    double calculateCharge(int days, int lateDays) const override {
        double base = days * dailyRate;
        double late = lateDays * (dailyRate * 2.0); // 2x penalty for cars
        return base + late;
    }
    std::string getType() const override { return "Car"; }
};

// 4. Electric Car
class ElectricCar : public MotorVehicle {
private:
    double batteryCapacityKWh;
    double chargePerKWh;

public:
    ElectricCar(const std::string& reg, double rate, double batteryPct,
                const std::string& date, double kwhCap, double kwhRate)
        : MotorVehicle(reg, rate, batteryPct, date),
          batteryCapacityKWh(kwhCap), chargePerKWh(kwhRate) {}

    double calculateCharge(int days, int lateDays) const override {
        double base = days * dailyRate;
        double late = lateDays * (dailyRate * 1.8);
        double energyCharge = batteryCapacityKWh * chargePerKWh * 0.2; // 20% top-up fee
        return base + late + energyCharge;
    }
    std::string getType() const override { return "Electric Car"; }
};

// 5. Demonstrated Extension: Electric Bus (Added without modifying existing classes!)
class ElectricBus : public Vehicle {
private:
    int passengerCapacity;
    double routePermitFee;

public:
    ElectricBus(const std::string& reg, double rate, int cap, double permit)
        : Vehicle(reg, rate), passengerCapacity(cap), routePermitFee(permit) {}

    double calculateCharge(int days, int lateDays) const override {
        return (days * dailyRate) + routePermitFee + (lateDays * dailyRate * 2.5);
    }
    std::string getType() const override { return "Electric Bus (Extension)"; }
};

int main() {
    std::cout << "CO4 - TASK 06: EXTENSIBLE VEHICLE RENTAL FLEET HIERARCHY\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "========================================================\n\n";

    std::vector<Vehicle*> fleet = {
        new Bicycle("BIKE-01", 150.0),
        new Car("TN-09-AB-1234", 2500.0, 85.0, "2026-12-01", 5),
        new ElectricCar("TN-09-EV-5678", 3200.0, 92.0, "2027-01-15", 60.0, 12.0),
        new ElectricBus("TN-09-BUS-9999", 8000.0, 45, 1500.0) // Demonstrated Extension!
    };

    struct RentalTest {
        int days;
        int lateDays;
    };

    RentalTest scenarios[] = {
        {3, 0}, // 3 days on time
        {2, 1}, // 2 days + 1 day late
        {5, 2}, // 5 days + 2 days late
        {1, 0}  // 1 day on time
    };

    std::cout << std::left << std::setw(26) << "Vehicle Type"
              << std::setw(18) << "Registration"
              << std::setw(12) << "Days (Late)"
              << "Total Rental Fee\n";
    std::cout << std::string(72, '-') << "\n";

    for (size_t i = 0; i < fleet.size(); ++i) {
        double fee = fleet[i]->calculateCharge(scenarios[i].days, scenarios[i].lateDays);
        std::cout << std::left << std::setw(26) << fleet[i]->getType()
                  << std::setw(18) << fleet[i]->getReg()
                  << scenarios[i].days << "d (" << scenarios[i].lateDays << "d late)   "
                  << "Rs " << std::fixed << std::setprecision(2) << fee << "\n";
    }

    for (Vehicle* v : fleet) delete v;

    return 0;
}
