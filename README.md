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
./tape_emulator ../data/input.bin ../data/output.bin ../data/tape.conf    
