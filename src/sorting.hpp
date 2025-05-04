#include "tape.hpp"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static size_t size_of_ram = 10;

size_t divide_for_blocks(size_t a, size_t b) {
    return (a + b - 1) / b;
}

void print_tape(Tape &t) {
    t.rewind();
    for (size_t i = 0; i < t.get_length(); ++i) {
        int j = t.read();
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
            int t = input.read();
            input.move_right();
            items.push_back(t);
        }
        sort(items.begin(), items.end());
        for (size_t j = 0; j < length; ++j) {
            (*block).write(items[j]);
            (*block).move_right();
        }
        (*block).rewind();
    }

    const size_t get_length() {
        return (*block).get_length();
    }

    void print_block() {
        (*block).rewind();

        for (size_t i = 0; i < (*block).get_length(); ++i) {
            std::cout << (*block).read() << " ";
            (*block).move_right();
        }
        std::cout << std::endl;
        (*block).rewind();
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
            std::cout << j << " block:" << std::endl;
            b.print_block();
            tape_container.push_back(std::move(b.get_tape()));
            b.~Block();
        }
        std::cout << std::endl;
        size_t g = 0;
        //maybe vise versa for rewind()
        for (size_t i = 0; i < length; ++i) {
            std::cout << "started number: " << i << std::endl;
            size_t buf = 0;
            std::pair<int, size_t> current;
            current.second = 0;
            std::vector<int> pretend_start;
            size_t k;
            std::cout << "pretend_start: ";
            for (k = 0; k < size_of_ram && k < number_of_blocks; ++k) {
                if ((*tape_container[k]).is_EOT()) {
                    buf++;
                    continue;
                }
                pretend_start.push_back((*tape_container[k]).read());
                std::cout << (*tape_container[k]).read() << " ";
            }
            std::cout << std::endl;
            auto min_it_start = std::min_element(pretend_start.begin(), pretend_start.end());
            current.first = *min_it_start;
            current.second = std::distance(pretend_start.begin(), min_it_start);
            std::cout << "pretend start number: " << current.second;
            std::cout << std::endl;
            pretend_start.clear();

            std::vector<int> pretend;
            size_t number_of_loops = 1;
            while (k != number_of_blocks) {
                size_t g = 0;
                std::cout << "additional: ";
                for (; g < size_of_ram && g + k < number_of_blocks;) {
                    if ((*tape_container[k + g]).is_EOT()) {
                        buf++;
                        continue;
                    }
                    pretend.push_back((*tape_container[k + g]).read());
                    std::cout << (*tape_container[k + g]).read() << " ";
                    g++;
                }
                std::cout << std::endl;
                k += g;
                auto min_it = std::min_element(pretend.begin(), pretend.end());
                if (*min_it < current.first) {
                    current.first = *min_it;
                    current.second = number_of_loops * size_of_ram + std::distance(pretend.begin(), min_it);
                    std::cout << "pretend additional number: " << std::distance(pretend.begin(), min_it);
                    std::cout << std::endl;
                }
                number_of_loops++;
                pretend.clear();
            }
            std::cout << "buf: " << buf << std::endl;
            std::cout << "min element: " << current.first << std::endl;
            std::cout << "moving tape number: " << current.second + buf<< std::endl;
            (*tape_container[current.second + buf]).move_right();
            std::cout << "writing followed " << std::endl;
            output.write(current.first);
            output.move_right();
            std::cout << "writing ended " << std::endl;
            std::cout << std::endl;
        }
    }

    ~Sort() = default;
};