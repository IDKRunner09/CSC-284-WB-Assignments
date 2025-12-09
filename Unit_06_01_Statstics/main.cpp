#include "Statistics.h"
#include <iostream>

using namespace std;

int main() {
    Statistics<int> intStats;
    intStats.addValue(19);
    intStats.addValue(8);
    intStats.addValue(9);

    Statistics<double> doubleStats;
    doubleStats.addValue(3.14);
    doubleStats.addValue(26.5);
    doubleStats.addValue(4.32);

    std::cout << "Int Stats: Min = " << intStats.getMin()
              << ", Max = " << intStats.getMax()
              << ", Avg = " << intStats.getAverage() << std::endl;

    std::cout << "Double Stats: Min = " << doubleStats.getMin()
              << ", Max = " << doubleStats.getMax()
    << ", Avg = " << doubleStats.getAverage() << std::endl;
}
