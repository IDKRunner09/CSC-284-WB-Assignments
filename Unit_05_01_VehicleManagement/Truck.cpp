#include "Truck.h"
#include <iostream>
#include <format>

Truck::Truck() : Vehicle(), loadCapacity(0.0) {}

Truck::Truck(const std::string& make, const std::string& model, int year, double mileage, double load_capacity) : 
	Vehicle(make, model, year, mileage), loadCapacity(load_capacity) {}

double Truck::getFuelEfficiency() const {
	return 15.0; //sample for less efficient
}

void Truck::displayInfo() const {
	Vehicle::displayInfo();
	std::cout << std::format("Type: Truck | Load Capacity: {}\nFuel Efficiency: {} km/l\n", loadCapacity, getFuelEfficiency());
}