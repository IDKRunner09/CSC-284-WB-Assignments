#include <iostream>
#include <vector>
#include "Vehicle.h"
#include "Car.h"
#include "Motorcycle.h"
#include "Truck.h"
#include "Fleet.h"

int main() {
	Fleet garage;
	garage.addVehicle(new Car("Ferarri", "LaFerarri", 2020, 2000, 2, false));
	garage.addVehicle(new Car("Tesla", "Model S", 2023, 25000, 4, true));
	garage.addVehicle(new Truck("Volvo", "F416", 2018, 80000, 20));
	garage.addVehicle(new Motorcycle("Ducati", "Dvail", 2016, 5000, false));
	std::cout << std::endl;
	// printing all vehicles from vector
	garage.displayAllVehicles();

	//printing average effieciency
	std::cout << "Average Fuel Effieciency: " << garage.getAverageEfficiency() << std::endl;

	
}