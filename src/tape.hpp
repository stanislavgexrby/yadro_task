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
    std::string filePath;
    size_t currentPos;

    static int readDelay;
    static int writeDelay;
    static int rewindDelay;
    static int shiftDelay;

    void applyDelay(int delay) const {
        if (delay > 0) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

public:
    static void setDelays(int rd, int wd, int rwd, int sd) {
        readDelay = rd;
        writeDelay = wd;
        rewindDelay = rwd;
        shiftDelay = sd;
    }

    Tape(const std::string& path, bool createIfNotExists = false)
                                : filePath(path), currentPos(0) {
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            if (createIfNotExists) {
                std::ofstream outFile(path, std::ios::binary);
                outFile.close();
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

    int read() {
        applyDelay(readDelay);
        file.seekg(currentPos * sizeof(int));
        int value;
        file.read(reinterpret_cast<char*>(&value), sizeof(int));
        if (!file) {
            throw std::runtime_error("Read error or end of tape");
        }
        currentPos++;
        return value;
    }

    void write(int value) {
        applyDelay(writeDelay);
        file.seekp(currentPos * sizeof(int));
        file.write(reinterpret_cast<const char*>(&value), sizeof(int));
        file.flush();
        currentPos++;
    }

    void moveLeft() {
        applyDelay(shiftDelay);
        if (currentPos == 0) {
            throw std::out_of_range("Cannot move left from position 0");
        }
        currentPos--;
    }

    void moveRight() {
        applyDelay(shiftDelay);
        currentPos++;
    }

    void rewind() {
        applyDelay(rewindDelay);
        currentPos = 0;
    }

    bool isEOT() {
        std::streampos originalPos = file.tellg();
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(originalPos);
        return currentPos * sizeof(int) >= fileSize;
    }
};

bool test();