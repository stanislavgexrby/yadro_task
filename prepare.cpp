#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::vector<int> data = {15, -3, 42, 0, 7, 100};

    std::ofstream out("data/input.bin", std::ios::binary);

    if(!out) {
        std::cerr << "Error creating file" << std::endl;
        return 1;
    }

    for(int num : data) {
        out.write(reinterpret_cast<const char*>(&num), sizeof(num));
    }

    return 0;
}