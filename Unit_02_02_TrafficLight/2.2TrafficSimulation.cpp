#include <iostream>
#include <vector>
 
using namespace std;

class TrafficLightSimulator{		
	private:
		vector<string> colors{"Red", "Green", "Yellow"};
		
	public:
		//print one cycles
		void simulator(int n) {
			for (int i = 0; i < colors.size(); i++) {
				cout << colors[i] << endl;
			}
		}

};

int main() {
	//variables
	TrafficLightSimulator sim;
	int cycles;

	//prompt and input for number of cycles
	printf("Enter the number of cycles: ");
	cin >> cycles;

	//print number of cycles
	int duration;
	for (int i = 0; i < cycles; i++) {
		sim.simulator(cycles);
	}
}