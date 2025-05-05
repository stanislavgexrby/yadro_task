#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <stdexcept>
#include <vector>
#include <functional>
#include <print>

class Tape {
    std::fstream file;
    std::string file_path;
    size_t current_pos;
    const size_t length;

    static int read_delay;
    static int write_delay;
    static int rewind_delay;
    static int shift_delay;

    void apply_delay(int delay) const {
        //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        //std::cout << "delay: " << delay << std::endl;
    }

public:
    static void set_delays(int rd, int wd, int rwd, int sd) {
        read_delay = rd;
        write_delay = wd;
        rewind_delay = rwd;
        shift_delay = sd;
    }

    Tape(const std::string& path, const size_t n = 100, bool create_if_no = false)
                                : file_path(path), length(n), current_pos(0) {
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            if (create_if_no) {
                std::ofstream out_file(path, std::ios::binary);
                out_file.close();
                file.open(path, std::ios::in | std::ios::out | std::ios::binary);
                if (!file.is_open()) {
                    throw std::runtime_error("Failed to create file: " + path);
                }
            } else {
                throw std::runtime_error("File not found: " + path);
            }
        }
        file.seekg(0, std::ios::beg);
        file.seekp(0, std::ios::beg);
    }

    ~Tape() {
        if (file.is_open()) {
            file.close();
        }
    }

    int read(int &value) {
        if (is_EOT()) {
            return 1;
        }
        apply_delay(read_delay);
        file.seekg(current_pos * sizeof(int));
        file.read(reinterpret_cast<char*>(&value), sizeof(int));
        if (!file) {
            std::cout << "pos: " << current_pos << " length: " << length << "path: " << file_path << std::endl;
            throw std::runtime_error("Read error");
        }
        return 0;
    }

    void write(int value) {
        apply_delay(write_delay);
        file.seekp(current_pos * sizeof(int));
        file.write(reinterpret_cast<const char*>(&value), sizeof(int));
        file.flush();
    }

    void move_left() {
        apply_delay(shift_delay);
        current_pos--;
    }

    void move_right() {
        apply_delay(shift_delay);
        current_pos++;
    }

    void rewind() {
        apply_delay(rewind_delay);
        current_pos = 0;
    }

    const size_t get_length() {
        return length;
    }

    bool is_EOT() {
        /*std::streampos original_pos = file.tellg();
        file.seekg(0, std::ios::end);

        size_t fileSize = file.tellg();
        file.seekg(original_pos);
        return current_pos * sizeof(int) >= fileSize;*/
        if (current_pos >= length || current_pos < 0) {
            return true;
        }
        return false;
        //return current_pos == length;
    }

    size_t get_pos() {
        return current_pos;
    }
};

bool test();