#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>
#include <print>

void LogAnalyzer::analyzeFile(const std::string& filename, const std::vector<std::string>& keywords) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::cout << "Processing: " << filename << std::endl;

    std::string line;

    // TODO: Read each line.
    while (std::getline(file, line)) {
        // For each keyword in 'keywords', check if it exists in the line.
        for (const std::string& keyword : keywords) {
            // If found, increment keywordCounts[keyword] (inside a mutex lock).
            if (line.find(keyword) != std::string::npos) {
                std::lock_guard<std::mutex> lock(countMutex);
                keywordCounts[keyword]++;
            }
        }
    }

    file.close();
}

void LogAnalyzer::printSummary() const {
    std::println("\n------------- Keyword Summary -------------\n");
    // TODO: Use std::for_each and a lambda to print keyword counts.
    std::for_each(keywordCounts.begin(), keywordCounts.end(), [](const auto& pair) {
        std::println("{}: {}", pair.first, pair.second); });
    std::println("---------------------------------------------\n");
}