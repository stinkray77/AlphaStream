# C++ Limit Order Book

A simulated high-frequency trading matching engine written in C++.

## Core Features

### 1. High-Performance Matching Engine
- **Limit Orders:** Standard buy/sell matching with Price-Time priority.
- **Market Orders:** Supports aggressive "Walking the Book" execution with Immediate-or-Cancel (IOC) logic to prevent locked markets.
- **Complexity:** $O(\log N)$ for order insertion using Red-Black Trees (`std::map`).

### 2. O(1) Order Management
- **Fast Cancellations:** Implemented a Hash Map sidecar (`std::unordered_map`) to track order locations.
- **Performance:** Reduced cancellation time from $O(N)$ (linear search) to $O(1)$ (constant time).
- **Memory Safety:** Automatic cleanup of "Ghost Orders" upon full execution to prevent memory leaks.

## Benchmarks
- **Throughput:** Processed 1,000,000 orders in **~642ms** on consumer hardware.
- **Optimization Strategy:** - Decoupled I/O from the critical path (logging is disabled during benchmarking).
    - Utilized `std::cin`/`std::cout` tie-breaking and memory pre-allocation.
    
## How to Run
```bash
# Compile both the Interface and Implementation
g++ main.cpp OrderBook.cpp -o engine

# Run the engine
./engine
```

## Roadmap
- [x] Core Matching Logic (Price-Time Priority)
- [x] Optimization: Switch to Red-Black Trees ($O(\log N)$)
- [x] Feature: Market Orders & Walking the Book
- [x] Feature: Order Cancellations ($O(1)$ Lookup)
- [x] Modularization: Separate Interface (.h) and Implementation (.cpp)
- [ ] Advanced: TCP/IP Socket Integration for Remote Order Entry

## System Architecture

[ Order Entry ]
      |
      v
[ Lookup Map (Hash Table) ] <-----> [ Order Book (Red-Black Tree) ]
   (Tracks ID -> Location)              (Bids & Asks Sorted by Price)
                                                |
                                                v
                                        [ Matching Engine ]
                                                |
                                                v
                                        [ Trade Execution ]