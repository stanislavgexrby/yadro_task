cmake_minimum_required(VERSION 3.20)
project(TapeSorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    test/test.cpp
)

add_executable(tape_emulator ${SOURCES})

add_custom_command(TARGET tape_emulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_BINARY_DIR}/data
)

install(TARGETS tape_emulator DESTINATION bin)