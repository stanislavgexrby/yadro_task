#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

int main() {
    //std::vector<int> data = {143, 78, 0, -10000,3,34,57,5,3,5,8,9,9,9,9,9,9};
    std::string input_line;
    std::vector<int> data;
    getline(std::cin, input_line);

    std::stringstream ss(input_line);
    int num;

    while (ss >> num) {
        data.push_back(num);
    }

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