#include <cstdio>
#include <filesystem>

bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}