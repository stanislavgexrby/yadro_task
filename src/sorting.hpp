#include "tape.hpp"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

void print_tape(Tape &t) {
    t.rewind();
    while (!t.is_EOT()) {
        int j = t.read();
        t.move_right();
        std::cout << j << " ";
    }
    std::cout << std::endl;
    t.rewind();
}

class Sort {
public:
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

    Sort(Tape &input, Tape &output){
        if (!input.is_EOT()) {
            int value = input.read();
            input.move_right();
            if (input.is_EOT()) {
                output.write(value);
                output.move_right();
            } else {
                auto higher_ptr = create_temp_tape();
                auto lower_ptr = create_temp_tape();
                while (!input.is_EOT()) {
                    int tmp = input.read();
                    input.move_right();
                    if (tmp > value) {
                        (*higher_ptr).write(tmp);
                        (*higher_ptr).move_right();
                    } else {
                        (*lower_ptr).write(tmp);
                        (*lower_ptr).move_right();
                    }
                }
                (*lower_ptr).rewind();
                (*higher_ptr).rewind();

                auto higher_sorted_ptr = create_temp_tape();
                auto lower_sorted_ptr = create_temp_tape();

                Sort((*lower_ptr), (*lower_sorted_ptr));
                Sort((*higher_ptr), (*higher_sorted_ptr));
                (*lower_sorted_ptr).rewind();
                (*higher_sorted_ptr).rewind();

                while (!(*lower_sorted_ptr).is_EOT()) {
                    int j = (*lower_sorted_ptr).read();
                    (*lower_sorted_ptr).move_right();
                    output.write(j);
                    output.move_right();
                }

                output.write(value);
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
    }

    ~Sort() = default;
};