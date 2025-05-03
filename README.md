Tape sort emulator for yadro task  
Build:  
mkdir build && cd build  
cmake -G Ninja ..  
Use:
leave build directory ( cd .. )
g++ prepare.cpp
//input numbers//
./a.out - in order to set data/input.bin - file for emulated tape
cd build
./tape_emulator ../data/input.bin ../data/output.bin ../data/tape.conf
