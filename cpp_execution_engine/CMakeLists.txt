cmake_minimum_required(VERSION 3.14)

project(AlphaStream_ExecutionEngine
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Tell CMake to use pkg-config
find_package(PkgConfig REQUIRED)

# 2. Use pkg-config to find the ZeroMQ C library (libzmq)
pkg_check_modules(ZeroMQ REQUIRED libzmq)

# 3. Add Homebrew prefix so CMake can find the C++ wrapper (cppzmq)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
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

# 4. Link the include directories and libraries found by pkg-config and cppzmq
target_include_directories(execution_engine PRIVATE ${ZeroMQ_INCLUDE_DIRS})
target_link_libraries(execution_engine PRIVATE cppzmq ${ZeroMQ_LIBRARIES})
