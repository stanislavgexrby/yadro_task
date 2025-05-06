# Tape Sort Emulator

## Requirements
- C++17 compiler
- CMake 3.20+
- Ninja

## Build
```bash
git clone https://github.com/stanislavgexrby/yadro_task
cd yadro_task
mkdir build && cd build
cmake -G Ninja ..
ninja
```

## Run
Firstly add data by running 
```bash
cd ..
g++ prepare.cpp
./a.out
```
and entering the length of input tape (this tape will be fulfilled with random ints).
Then create tape.conf file with delay preferences in data directory (example of it is already in data folder).
Then
```bash
cd build
./tape_emulator ../data/input.bin ../data/output.bin ../data/tape.conf
```
