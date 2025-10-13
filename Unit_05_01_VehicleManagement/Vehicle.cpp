#include "Vehicle.h"
#include <iostream>
#include <format>

int Vehicle::vehicleCount = 0;

Vehicle::Vehicle() : make(""), model(""), year(0), mileage(0.0) {
	vehicleCount++;
}

Vehicle::Vehicle(const std::string& make, const std::string& model, int year, double mileage) :
	make(make), model(model), year(year), mileage(mileage) {
	vehicleCount++;
}

Vehicle::~Vehicle() {
	std::cout << "Destroying Vehicle: " << make << " " << model << std::endl;
	vehicleCount--;
}

std::string Vehicle::getMake() const { return make; }
std::string Vehicle::getModel() const { return model; }
int Vehicle::getYear() const { return year; }
double Vehicle::getMileage() const { return mileage; }

void Vehicle::setMake(const std::string& make) { this->make = make; }
void Vehicle::setModel(const std::string& model) { this->model = model; }
void Vehicle::setYear(int year) { this->year = year; }
void Vehicle::setMileage(double mileage) { this->mileage = mileage; }

void Vehicle::displayInfo() const {
	std::cout << std::format("{} {} {} | Mileage: {} km\n", year, make, model, mileage);
}

int Vehicle::getVehicleCount() { return vehicleCount; };
