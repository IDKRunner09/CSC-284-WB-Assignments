#ifndef Statistics_hpp
#define Statistics_hpp
#include <vector>

template <typename T>
T getMin(const std::vector<T>& values) {
    T minimumValue = values[0];
    for (const auto val : values) {
        if (val < minimumValue) { minimumValue = val; }
    }
    return minimumValue;
}

template <typename T>
T getMax(const std::vector<T>& values) {
    T maximumValue = values[0];
    for (const auto val : values) {
        if (val > maximumValue) { maximumValue = val; }
    }
    return maximumValue;
}

template <typename T>
double getAverage(const std::vector<T>& values) {
    T sum = 0;
    for (const auto& val : values) {
        sum += val;
    }
    return sum/values.size();
}

template <typename T>
class Statistics {
private:
    std::vector<T> data;
public:
    void addValue(const T& value) {
        data.push_back(value);
    }
    T getMin() const {
        return::getMin(data);
    }
    T getMax() const {
        return::getMax(data);
    }
    double getAverage() const {
        return::getAverage(data);
    }
};
#endif
