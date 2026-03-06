#pragma once // Prevents the file from being included twice
#include <map>
#include <vector>
#include <iostream>
#include <limits>
#include <unordered_map>

enum class OrderType {
    LIMIT,
    MARKET
};

struct Order {
    int id;             // Unique ID for the order
    OrderType type;
    bool is_buy;        // true = Buy (Bid), false = Sell (Ask)
    int price;          // In cents (e.g., $100.00 -> 10000)
    int quantity;       // Number of shares
};

struct OrderEntry {
    bool is_buy;
    int price;
};

class OrderBook {
    // Bids : High to Low (Standard map sorts Low to High, so I use std::greater)
    std::map<int, std::vector<Order>, std::greater<int>> bids;

    // Asks: Low to High (Default behavior)
    std::map<int, std::vector<Order>> asks;

    // Track orders
    std::unordered_map<int, OrderEntry> order_lookup;

    public: 
        void add_order(int id, bool is_buy, int price, int quantity, OrderType type);
        void cancel_order(int id);
        void match_orders();
        void print_book();
};