#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

class FileHandler {
public:
    FileHandler(const fs::path& path) : directory_path(path) {
        if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
            cerr << "Error: Invalid directory path.\n";
            exit(EXIT_FAILURE);
        }
    }

    void sortFilesByName() {
        for (const auto& entry : fs::directory_iterator(directory_path))
            files.push_back(entry.path().filename().string());
        
        sort(files.begin(), files.end());
        displayFiles();
    }

    void sortFilesByModifiedTime() {
        for (const auto& entry : fs::directory_iterator(directory_path))
            files.push_back(entry.path().filename().string());

        sort(files.begin(), files.end(), [this](const auto& a, const auto& b) {
            return fs::last_write_time(directory_path / a) > fs::last_write_time(directory_path / b);
        });

        displayFiles();
    }

    void findFileByName(const string& fileName) {
        auto it = find(files.begin(), files.end(), fileName);
        if (it != files.end())
            cout << "File found: " << *it << '\n';
        else
            cout << "File not found.\n";
    }

private:
    void displayFiles() {
        for (const auto& file : files)
            cout << file << '\n';
    }

    fs::path directory_path;
    vector<string> files;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <directory_path> [-s | -f <file_name>]\n";
        exit(EXIT_FAILURE);
    }

    fs::path directory_path(argv[1]);
    FileHandler fileHandler(directory_path);

    if (argc == 2 || (argc == 3 && string(argv[2]) == "-s"))
        fileHandler.sortFilesByName();
    else if (argc == 4 && string(argv[2]) == "-f")
        fileHandler.findFileByName(argv[3]);
    else if (argc == 3 && string(argv[2]) == "-f") {
        cerr << "Error: File name not provided.\n";
        exit(EXIT_FAILURE);
    }
    else {
        cerr << "Error: Invalid arguments.\n";
        exit(EXIT_FAILURE);
    }

    return 0;
}
