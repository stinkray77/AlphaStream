#include <iostream>
#include <chrono>
#include <vector>
#include "OrderBook.h" 

int main() {
    OrderBook book; 
    
    // 1. Pre-generate dummy orders
    int iterations = 10000000;
    
    // Updated struct to match your signature (int price, int quantity)
    struct OrderData { int id; bool buy; int price; int qty; };
    std::vector<OrderData> orders;
    orders.reserve(iterations);
    
    for(int i = 0; i < iterations; ++i) {
        // Generating integer price (e.g., 100) instead of double
        orders.push_back({i, (i % 2 == 0), 100 + (i % 10), 100});
    }

    // 2. Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    // 3. Run the "hot path" loop
    for(const auto& o : orders) {
        // CORRECTED SIGNATURE: add_order(id, is_buy, price, quantity, type)
        book.add_order(o.id, o.buy, o.price, o.qty, OrderType::LIMIT);
    }

    // 4. Stop Timer
    auto end = std::chrono::high_resolution_clock::now();

    // 5. Calculate Metrics
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_latency = (double)duration / iterations;

    std::cout << "Total time for " << iterations << " orders: " << duration << " us" << std::endl;
    std::cout << "Average Latency per Order: " << avg_latency << " microseconds" << std::endl;

    return 0;
}