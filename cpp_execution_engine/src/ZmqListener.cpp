#include <zmq.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "OrderBook.h"

using json = nlohmann::json;

struct AlphaSignal
{
    std::string ticker;
    double sentiment_score;
    std::string action;
};

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::sub);
    socket.connect("tcp://localhost:5555");
    socket.set(zmq::sockopt::subscribe, "SIGNAL");

    OrderBook limit_order_book;
    int order_id_counter = 0; // Changed to int to match your add_order signature

    std::cout << "Execution Engine listening for signals..." << std::endl;

    while (true)
    {
        zmq::message_t update;
        auto res = socket.recv(update, zmq::recv_flags::none);

        if (res)
        {
            std::string msg_str = update.to_string();
            std::string json_payload = msg_str.substr(7);

            try
            {
                json j = json::parse(json_payload);
                AlphaSignal signal = {j["ticker"], j["sentiment_score"], j["action"]};

                // 1. Prepare data for your specific add_order signature
                int id = ++order_id_counter;
                bool is_buy = (signal.action == "BUY");
                int price = 150; // Placeholder: your add_order expects int price
                int quantity = static_cast<int>(std::abs(signal.sentiment_score) * 100);

                // 2. Call your existing add_order with the 5 required arguments
                // I am assuming OrderType::LIMIT exists based on your LOB project
                limit_order_book.add_order(id, is_buy, price, quantity, OrderType::LIMIT);

                std::cout << "[EXECUTED] " << (is_buy ? "BUY" : "SELL")
                          << " | Ticker: " << signal.ticker
                          << " | Qty: " << quantity << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Pipeline Error: " << e.what() << '\n';
            }
        }
    }
    return 0;
}
