#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

class FileHandler {
public:
    FileHandler(const fs::path& path) : directory_path(path) {
        if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
            std::cerr << "Error: Invalid directory path.\n";
            exit(EXIT_FAILURE);
        }
    }

    void sortFilesByName() {
        for (const auto& entry : fs::directory_iterator(directory_path))
            files.push_back(entry.path().filename().string());
        
        std::sort(files.begin(), files.end());
        displayFiles();
    }

    void sortFilesByModifiedTime() {
        for (const auto& entry : fs::directory_iterator(directory_path))
            files.push_back(entry.path().filename().string());

        std::sort(files.begin(), files.end(), [this](const auto& a, const auto& b) {
            return fs::last_write_time(directory_path / a) > fs::last_write_time(directory_path / b);
        });

        displayFiles();
    }

    void findFileByName(const std::string& fileName) {
        auto it = std::find(files.begin(), files.end(), fileName);
        if (it != files.end())
            std::cout << "File found: " << *it << '\n';
        else
            std::cout << "File not found.\n";
    }

private:
    void displayFiles() {
        for (const auto& file : files)
            std::cout << file << '\n';
    }

    fs::path directory_path;
    std::vector<std::string> files;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path> [-s | -f <file_name>]\n";
        exit(EXIT_FAILURE);
    }

    fs::path directory_path(argv[1]);
    FileHandler fileHandler(directory_path);

    if (argc == 2 || (argc == 3 && std::string(argv[2]) == "-s"))
        fileHandler.sortFilesByName();
    else if (argc == 4 && std::string(argv[2]) == "-f")
        fileHandler.findFileByName(argv[3]);
    else if (argc == 3 && std::string(argv[2]) == "-f") {
        std::cerr << "Error: File name not provided.\n";
        exit(EXIT_FAILURE);
    }
    else {
        std::cerr << "Error: Invalid arguments.\n";
        exit(EXIT_FAILURE);
    }

    return 0;
}
