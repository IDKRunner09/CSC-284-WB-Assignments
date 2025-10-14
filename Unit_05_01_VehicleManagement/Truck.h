#ifndef TRUCK_H
#define TRUCK_H
#include "Vehicle.h"

class Truck : public Vehicle{
	private:
		double loadCapacity;

	public:
		Truck();
		Truck(const std::string& make, const std::string& model, int year, double mileage, double load_capacity);

		double getFuelEfficiency() const override;
		void displayInfo() const override;
};

#endif