#include "Car.h"
#include <iostream>
#include <format>

Car::Car() : Vehicle(), numDoors(4), isElectric(false) {};

Car::Car(const std::string& make, const std::string& model, int year, double mileage, int doors, bool electric)
    : Vehicle(make, model, year, mileage), numDoors(doors), isElectric(electric) {}


double Car::getFuelEfficiency() const
{
    return isElectric ? 0.0 : 30.0;
}

void Car::displayInfo() const {
    Vehicle::displayInfo();
    std::cout << std::format("Type: Car | Doors: {} | Electic: {}\nFuel Efficiency: {}\n", numDoors, (isElectric ? "Yes" : "No"), getFuelEfficiency());
}