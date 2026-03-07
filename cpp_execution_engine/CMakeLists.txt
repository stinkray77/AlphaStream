cmake_minimum_required(VERSION 3.14)

project(AlphaStream_ExecutionEngine
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Homebrew paths explicitly for macOS (Covers both Apple Silicon and Intel)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

# Now CMake can successfully find the packages
find_package(ZeroMQ REQUIRED)
find_package(cppzmq REQUIRED)

set(COMPILER_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -O3
    -march=native
)

set(SOURCES
    src/ZmqListener.cpp
)

add_executable(execution_engine ${SOURCES})
target_compile_options(execution_engine PRIVATE ${COMPILER_FLAGS})

# Link against the cppzmq target
target_link_libraries(execution_engine PRIVATE cppzmq)
