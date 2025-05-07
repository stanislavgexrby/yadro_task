#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <chrono>
#include <limits>

int main() {
    //std::vector<int> data = {143, 78, 0, -10000,3,34,57,5,3,5,8,9,9,9,9,9,9};
    std::vector<int> data;
    int n;
    std::cin >> n;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    std::uniform_int_distribution<int> distrib(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    );

    for (int i = 0; i < n; ++i) {
        data.push_back(distrib(gen));
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