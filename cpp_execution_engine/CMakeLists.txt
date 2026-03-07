cmake_minimum_required(VERSION 3.14)

project(AlphaStream_ExecutionEngine
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Define search paths FIRST so all subsequent find_package commands work on macOS
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

# 2. Tell CMake to use pkg-config
find_package(PkgConfig REQUIRED)

# 3. Find nlohmann_json (This will now work because Homebrew paths are set)
find_package(nlohmann_json REQUIRED)

# 4. Use pkg-config to find the ZeroMQ C library (libzmq)
pkg_check_modules(ZeroMQ REQUIRED libzmq)

# 5. Find the C++ wrapper (cppzmq)
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
    src/OrderBook.cpp
)

add_executable(execution_engine ${SOURCES})
target_compile_options(execution_engine PRIVATE ${COMPILER_FLAGS})

# 6. Link the include directories and libraries
target_include_directories(execution_engine PRIVATE ${ZeroMQ_INCLUDE_DIRS} include/)
target_link_libraries(execution_engine PRIVATE cppzmq ${ZeroMQ_LIBRARIES} nlohmann_json::nlohmann_json)
