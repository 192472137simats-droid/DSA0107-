# Task 06 - Day-4 Design Challenge: Vehicle Rental Fleet
**Student Name:** G SAI TEJA  
**Register Number:** 192472137  

## Description
Independently engineers an extensible polymorphic vehicle rental hierarchy:
- Abstract base class `Vehicle` with pure virtual function `calculateRentalCharge`.
- Specialized derived classes: `Bicycle`, `MotorVehicle`, `Car`, `ElectricCar`.
- Seamless extension: Adds `ElectricBus` without modifying existing working classes, satisfying the Open/Closed Principle (OCP).

## Compilation and Execution
```bash
g++ -std=c++17 -Wall -Wextra -o VehicleRentalFleet.exe VehicleRentalFleet.cpp
./VehicleRentalFleet.exe
```
