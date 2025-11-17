#include <iostream>
#include <fstream>
#include <filesystem>
#include <print>
#include "json.hpp"
namespace fs = std::filesystem;
using json = nlohmann::json;


json getReplacementObject(const std::string& value) {
    static const std::unordered_map<std::string, json> replacements = {
        {"enabled", {
            {"status", "enabled"},
            {"since", "2024-10-01"}
        }},
        {"manage_users", {
            {"permission", "manage_users"},
            {"granted_at", "2024-10-05"},
            {"level", "full"}
        }}
    };

    auto it = replacements.find(value);
    if (it != replacements.end()) {
        return it->second;
    }
    return nullptr;
}


void transformJson(json& j) {
    if (j.is_object()) {
        for (auto& item : j.items()) {
            transformJson(item.value());
        }
    }
    else if (j.is_array()) {
        for (auto& element : j) {
            transformJson(element);
        }
    }
    else if (j.is_string()) {
        std::string val = j.get<std::string>();

        // Email update
        if (val.find("@company.com") != std::string::npos) {
            val = val.substr(0, val.find("@company.com")) + "@newcompany.com";
            j = val;
        }

        // Value replacements
        json replacement = getReplacementObject(val);
        if (!replacement.is_null()) {
            j = replacement;
        }
    }
}

std::string readFile(const fs::path& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Could not open file.");
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string timestamp() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &tm);
    return std::string(buffer);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <path_to_json_file>" << std::endl;
		return 1;
	} 

    fs::path inputDir = argv[1];
    if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
        std::println("Error: Invalid directory.");
        return 1;
    }

    fs::path outputDir = inputDir.parent_path() / timestamp();
    fs::create_directories(outputDir);

    std::print("Input Directories: ", inputDir);

	for (const auto& entry : fs::recursive_directory_iterator(inputDir)) {
		if (entry.path().extension() == ".json") {
            fs::path rel = fs::relative(entry.path(), inputDir);
            fs::path dest = outputDir / rel;

            if (entry.is_directory()) {
                fs::create_directories(dest);
                continue;
            }

            if (entry.path().extension() == ".json") {
                try {
                    std::string text = readFile(entry.path());
                    json j = json::parse(text);

                    transformJson(j);

                    fs::create_directories(dest.parent_path());
                    
                    std::ofstream out(dest);
                    out << std::setw(4) << j;

                    std::println("Processed JSON: ", rel);
                }
                catch (...) {
                    std::println("Failed to process JSON (copied raw): ", rel);
					fs::create_directories(dest.parent_path());
					fs::copy_file(entry.path(), dest, fs::copy_options::overwrite_existing);
                }
            }
            else {
				fs::create_directories(dest.parent_path());
                fs::copy_file(entry.path(), dest, fs::copy_options::overwrite_existing);
            }
		}
	}
}