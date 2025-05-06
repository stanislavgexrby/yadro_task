#include "tape.hpp"

#include <algorithm>
#include <ranges>
#include <filesystem>

namespace fs = std::filesystem;

static size_t size_of_ram = 10;

size_t divide_for_blocks(size_t a, size_t b) {
    return (a + b - 1) / b;
}

void print_tape(Tape &t) {
    t.rewind();
    for (size_t i = 0; i < t.get_length(); ++i) {
        int j ;
        t.read(j);
        t.move_right();
        std::cout << j << " ";
    }
    std::cout << std::endl;
    t.rewind();
}

int count_tmp_tapes() {
    std::string dir_path = "../tmp/";
    int max_number = 0;

    for (const auto& entry : fs::directory_iterator(dir_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bin") {
            std::string stem = entry.path().stem().string();
            int num = std::stoi(stem);
            max_number = std::max(max_number, num);
        }
    }

    return max_number;
}

std::unique_ptr<Tape> create_temp_tape(int length = 100) {
    std::string tmp_path = "../tmp/" + std::to_string(count_tmp_tapes() + 1) + ".bin";
    return std::make_unique<Tape>(tmp_path, length, true);
}

class Block {
    std::unique_ptr<Tape> block;
public:
    Block(Tape &input) {
        int length = std::min(input.get_length(), size_of_ram);
        block = std::move(create_temp_tape(length));
        std::vector<int> items;
        for (size_t i = 0; i < length; ++i) {
            int t;
            input.read(t);
            input.move_right();
            items.push_back(t);
        }
        sort(items.begin(), items.end());
        for (size_t j = 0; j < length - 1; ++j) {
            (*block).write(items[j]);
            (*block).move_right();
        }
        (*block).write(items[length - 1]);
    }

    const size_t get_length() {
        return (*block).get_length();
    }

    std::unique_ptr<Tape> get_tape() {
        return std::move(block);
    }

    //~Block() = default;
};

class Sort {
public:
    Sort(Tape &input, Tape &output){
        size_t length = input.get_length();
        size_t number_of_blocks = divide_for_blocks(length, size_of_ram);
        std::vector<std::unique_ptr<Tape>> tape_container;

        for (size_t j = 0; j < number_of_blocks; ++j) {
            Block b(input);
            tape_container.push_back(std::move(b.get_tape()));
            b.~Block();
        }
        for (size_t g = 0; g < length - 1; ++g) {
            output.move_right();
        }

        size_t i = length - 1;
        for (; i < length && i >= 0; i--) {
            std::pair<int, size_t> current;
            size_t k;
            bool found = false;

            for (k = 0; k < size_of_ram; k++) {
                int num;
                if ((*tape_container[k]).read(num) == 0) {
                    if (!found || num > current.first) {
                        current.first = num;
                        current.second = k;
                        found = true;
                    }
                }
            }

            size_t g;
            while (k != number_of_blocks) {
                for (g = 0; g < size_of_ram - 1 && g + k < number_of_blocks; ++g) {
                    int num;
                    if ((*tape_container[k + g]).read(num) == 0) {
                        if (!found || num > current.first) {
                            current.first = num;
                            current.second = k + g;
                        }
                    }
                }
                k += g;
            }
            (*tape_container[current.second]).move_left();
            output.write(current.first);
            output.move_left();
        }
    }

    ~Sort() = default;
};