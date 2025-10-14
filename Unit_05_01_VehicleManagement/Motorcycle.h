#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H
#include "Vehicle.h"
class Motorcycle : public Vehicle {
	private:
		bool hasSidecar;
	
	public:
		Motorcycle();
		Motorcycle(const std::string& make, const std::string& model, int year, double mileage, bool hasSideCar);
		double getFuelEfficiency() const override;
		void displayInfo() const override;
};

#endif