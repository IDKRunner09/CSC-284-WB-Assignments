#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>
class Vehicle {
	private:
		std::string make, model;
		int year;
		double mileage;
		static int vehicleCount;

	public:
		//constructors
		Vehicle();
		Vehicle(const std::string& make, const std::string& model, int year, double mileage);
		
		//destrucor
		~Vehicle();

		//getteers
		std::string getMake() const;
		std::string getModel() const;
		int getYear() const;
		double getMileage() const;
		
		//setters
		void setMake(const std::string& m);
		void setModel(const std::string& m);
		void setYear(int y);
		void setMileage(double m);

		//virtual functions
		virtual void displayInfo() const;
		virtual double getFuelEfficiency() const = 0;

		//getter for vehicle count
		static int getVehicleCount();
};
#endif 

