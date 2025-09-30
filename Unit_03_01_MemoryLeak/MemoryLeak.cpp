#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

int main() {

	vector<int*> memory;
	const size_t one_mb = 1024 * 1024/sizeof(int);
	int* block;

	for (int i = 0; i < 15; i++) {
		block = new int[one_mb];
		memory.push_back(block);
		printf("%d sec \t1 mb of memory allocated\n", i);
		this_thread::sleep_for(chrono::seconds(1));
	}
	// the for looop below deletes the memory
	for (size_t i = 0; i < memory.size(); i++) {
		delete[] memory[i];
		this_thread::sleep_for(chrono::seconds(1));
	}
}
