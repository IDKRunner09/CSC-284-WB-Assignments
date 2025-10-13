#ifndef CAR_H
#define CAR_H
#include "Vehicle.h"

class Car : public Vehicle {
	private:
		int numDoors;
		bool isElectric;

	public: 
		Car();
		Car(const std::string& make, const std::string& model, int year, double mileage, int doors, bool electric);

		double getFuelEfficiency() const override;
		void displayInfo() const override;
};

#endif