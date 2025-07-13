#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

void copyFile(const std::string& sourcePath, const std::string& destPath) {
    std::ifstream src(sourcePath, std::ios::binary);
    std::ofstream dst(destPath, std::ios::binary);
    dst << src.rdbuf();
}

void copyFiles(const std::string& sourceFolder, const std::string& destinationFolder) {
    DIR* dir = opendir(sourceFolder.c_str());
    if (!dir) {
        std::cerr << "Failed to open source folder: " << sourceFolder << std::endl;
        return;
    }

    MKDIR(destinationFolder.c_str());

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") continue;

        std::string sourcePath = sourceFolder + "/" + name;
        std::string destPath = destinationFolder + "/" + name;

        struct stat st;
        if (stat(sourcePath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
            copyFile(sourcePath, destPath);
        }
    }

    closedir(dir);
    std::cout << "Files copied successfully.\n";
}

int main() {
    std::string source = "./source_folder";       // Replace with your source folder path
    std::string destination = "./destination_folder"; // Replace with your destination folder path

    copyFiles(source, destination);

    return 0;
}
