#include "LogAnalyzer.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <print>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_directory>\n";
        return 1;
    }

    std::string folderPath = argv[1];
    LogAnalyzer analyzer;
    std::vector<std::string> keywords = { "WARN", "ERROR", "FATAL", "INFO", "DEBUG", "TRACE" /* TODO: Add the rest of the error levels */ };
    std::vector<std::thread> threads;
	int file_count = 0;

    std::cout << "Analyzing folder: " << folderPath << "\n\n";
    // TODO: Use std::filesystem to iterate through all files in the folder.
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        // For each file with ".log" extension:
        if (entry.is_regular_file() && entry.path().extension() == ".log") {
			++file_count;
            //   - Create a thread calling analyzer.analyzeFile(filename, keywords)
            //   - Store the thread in 'threads' vector.
            threads.emplace_back(&LogAnalyzer::analyzeFile, &analyzer, entry.path().string(), keywords);
			std::cout << "Spawned thread for file: " << entry.path().string() << std::endl;
        }
    }
    // TODO: Join all threads after launching.
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    analyzer.printSummary();
	std::println("Analysis Complete.\nProcessed: {}\n", file_count); 
    return 0;

}
