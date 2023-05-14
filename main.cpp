#include <iostream>
#include <filesystem>
#include <cstdlib>

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::filesystem::path currentPath=std::filesystem::current_path();
    std::cout << currentPath << std::endl;
    std::cout<<std::getenv("HOSTNAME")<<std::endl;

    return 0;
}
