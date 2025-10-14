#ifndef FLEET_H
#define FLEET_H
#include <vector>
#include "Vehicle.h"
class Fleet {
	private:
		std::vector<Vehicle*> fleet;
	public:
		Fleet();
		void addVehicle(Vehicle* vehicle);
		void displayAllVehicles() const;
		double getAverageEfficiency() const;
};
#endif