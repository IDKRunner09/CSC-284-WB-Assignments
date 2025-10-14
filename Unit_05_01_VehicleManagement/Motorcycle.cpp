#include "Motorcycle.h"
#include <iostream>
#include <format>

Motorcycle::Motorcycle() : Vehicle(), hasSidecar(false) {}
Motorcycle::Motorcycle(const std::string& make, const std::string& model, int year, double mileage, bool hasSideCar) : Vehicle(make, model, year, mileage), hasSidecar(hasSideCar) {}

void Motorcycle::displayInfo() const {
	Vehicle::displayInfo();
	std::cout << std::format("Type: Motorcycle | Sidecar: {}\nFuel Efficiency: {} km/l\n", (hasSidecar ? "Yes" : "No"), getFuelEfficiency());
}

double Motorcycle::getFuelEfficiency() const {
	return 35.0; //for higher efficiency
}