#include "Fleet.h"
#include <vector>
#include <iostream>

Fleet::Fleet() {}

void Fleet::addVehicle(Vehicle* vehicle) {
	fleet.push_back(vehicle);
	std::cout << "Vehicle added\n";
}

void Fleet::displayAllVehicles() const {
	std::cout << "--- Fleet Vehicles---" << std::endl;
	for (Vehicle* v : fleet) {
		v->displayInfo();
		std::cout << std::endl;
	}
}

double Fleet::getAverageEfficiency() const {
	double totalEfficiency = 0;
	if (fleet.empty()) {
		return 0.0;
	}
	else {
		for (Vehicle* v : fleet) {
			totalEfficiency += v->getFuelEfficiency();
		}
		return totalEfficiency / fleet.size();
	}
}