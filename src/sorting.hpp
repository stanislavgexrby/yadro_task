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
        /*if (!input.is_EOT()) {
            int value1 = input.read();
            input.move_right();
            if (input.is_EOT()) {
                output.write(value1);
                output.move_right();
            } else {
                int value2 = input.read();
                input.move_right();
                if (input.is_EOT()) {
                    if (value1 < value2) {
                        output.write(value1);
                        output.move_right();
                        output.write(value2);
                    } else {
                        output.write(value2);
                        output.move_right();
                        output.write(value1);
                    }
                    output.rewind();
                } else {
                    auto higher_ptr = create_temp_tape();
                    auto lower_ptr = create_temp_tape();
                    while (!input.is_EOT()) {
                        int tmp = input.read();
                        input.move_right();
                        if (tmp > value1) {
                            (*higher_ptr).write(tmp);
                            (*higher_ptr).move_right();
                        } else {
                            (*lower_ptr).write(tmp);
                            (*lower_ptr).move_right();
                        }
                    }
                    (*lower_ptr).rewind();
                    (*higher_ptr).rewind();

                    std::unique_ptr<Tape> lower_sorted_ptr;
                    std::unique_ptr<Tape> higher_sorted_ptr;

                    if (!(*lower_ptr).is_EOT()) {
                        lower_sorted_ptr = create_temp_tape();
                        Sort((*lower_ptr), (*lower_sorted_ptr), size_of_ram);
                        (*lower_sorted_ptr).rewind();
                    } else {
                        lower_sorted_ptr = std::move(lower_ptr);
                    }
                    if (!(*higher_ptr).is_EOT()) {
                        higher_sorted_ptr = create_temp_tape();
                        Sort((*higher_ptr), (*higher_sorted_ptr), size_of_ram);
                        (*higher_sorted_ptr).rewind();
                    } else {
                        higher_sorted_ptr = std::move(higher_ptr);
                    }

                    while (!(*lower_sorted_ptr).is_EOT()) {
                        int j = (*lower_sorted_ptr).read();
                        (*lower_sorted_ptr).move_right();
                        output.write(j);
                        output.move_right();
                    }

                    output.write(value1);
                    output.move_right();

                    while (!(*higher_sorted_ptr).is_EOT()) {
                        int i = (*higher_sorted_ptr).read();
                        (*higher_sorted_ptr).move_right();
                        output.write(i);
                        output.move_right();
                    }

                    output.rewind();
                }
            }
        }*/
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
        size_t g = 0;
        //maybe vise versa for rewind()
        for (size_t i = 0; i < length; ++i) {
            std::pair<int, size_t> current;
            std::vector<int> pretend_start;
            size_t k;
            for (k = 0; k < size_of_ram && k < number_of_blocks; ++k) {
                pretend_start.push_back((*tape_container[k]).read());
            }
            auto min_it_start = std::min_element(pretend_start.begin(), pretend_start.end());
            current.first = *min_it_start;
            current.second = std::distance(pretend_start.begin(), min_it_start);

            bool over = (k == number_of_blocks) ? true : false;
            std::vector<int> pretend;
            while (true) {
                size_t g = 0;
                for (; g < size_of_ram && g + k < number_of_blocks;) {
                    pretend.push_back((*tape_container[g]).read());
                    g++;
                }
                k += g;
                auto min_it = std::min_element(pretend.begin(), pretend.end());
                if (*min_it <= current.first) {
                    current.first = *min_it;
                    current.second = std::distance(pretend.begin(), min_it);
                }
                pretend.clear();
                if (k == number_of_blocks) {
                    break;
                }
            }
            std::cout << "min element: " << current.first << std::endl;
            (*tape_container[current.second]).move_right();

            output.write(current.first);
            output.move_right();
        }
    }

    ~Sort() = default;
};