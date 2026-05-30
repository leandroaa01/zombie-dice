cmake_minimum_required(VERSION 3.22.1)
project(zdice VERSION 1.0 DESCRIPTION "Fork of Zombie Dice board game." LANGUAGES CXX)

# Set the C++ language standard.
#
# Using C++17.
set(CMAKE_CXX_STANDARD 17)
# Require the compiler to support exactly this standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flag to set the binaries output directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Flag to generate compile_commands.json (Useful for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adds the includes directory.
include_directories(include lib)

# Recursively searches for all .cpp files in src/ and subdirectories.
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Create the executable called 'zdice' using the source files found and
# main.cpp.
#
# NOTE: main.cpp is already in SOURCES, explicitly including it is not
# necessary (But I want to leave it here because yes :D).
add_executable(zdice ${SOURCES} src/main.cpp)

# Adds the include directories to the 'forest' target.
target_include_directories(zdice PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set compilation flags.
target_compile_options(zdice PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-deprecated
    -O2
)