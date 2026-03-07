cmake_minimum_required(VERSION 3.14)

find_package(ZeroMQ REQUIRED)
find_package(cppzmq REQUIRED)

# Define the project and the language (C++)
project(AlphaStream_ExecutionEngine
        VERSION 1.0.0
        LANGUAGES CXX)

# Enforce modern C++ standards. C++17 is the current baseline for most financial systems.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define strict warning and optimization flags for the compiler
set(COMPILER_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror       # Treats all warnings as errors. Crucial for trading systems.
    -O3           # Maximum optimization for execution speed.
    -march=native # Optimizes the binary for the specific CPU architecture compiling it.
)

# Add your source files here. 
# Replace these with the actual names of your Limit Order Book files.
set(SOURCES
    src/main.cpp
    src/OrderBook.cpp
)

# Create the executable target
add_executable(execution_engine ${SOURCES})

# Apply the strict flags to this specific executable
target_compile_options(execution_engine PRIVATE ${COMPILER_FLAGS})

# Tell CMake where to find your header files (e.g., OrderBook.h)
target_include_directories(execution_engine PRIVATE include/)

target_link_libraries(execution_engine PRIVATE libzmq)
target_link_libraries(execution_engine PRIVATE cppzmq)
