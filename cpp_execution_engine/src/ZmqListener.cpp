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
    socket.connect("tcp://inference-service:5555");
    socket.set(zmq::sockopt::subscribe, "");

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

            // Find where the JSON starts '{' instead of hardcoding 7
            size_t json_start = msg_str.find('{');
            if (json_start == std::string::npos)
                continue;

            std::string json_payload = msg_str.substr(json_start);

            try
            {
                // Parse the raw string
                json j = json::parse(json_payload);

                // Check if the keys actually exist to avoid the 'null' error
                if (j.contains("ticker") && j.contains("sentiment_score") && j.contains("action"))
                {

                    // Explicitly cast them to ensure types match your struct
                    std::string ticker = j.at("ticker").get<std::string>();
                    double sentiment = j.at("sentiment_score").get<double>();
                    std::string action = j.at("action").get<std::string>();

                    // Now trigger your OrderBook logic
                    int id = ++order_id_counter;
                    bool is_buy = (action == "BUY");
                    int price = 150;
                    int quantity = static_cast<int>(std::abs(sentiment) * 100);

                    limit_order_book.add_order(id, is_buy, price, quantity, OrderType::LIMIT);

                    std::cout << "[SUCCESS] Order Placed: " << (is_buy ? "BUY" : "SELL")
                              << " " << quantity << " shares of " << ticker << std::endl;
                }
                else
                {
                    std::cerr << "[JSON ERROR] Missing required keys in payload." << std::endl;
                }
            }
            catch (const json::exception &e)
            {
                std::cerr << "[SERIALIZATION ERROR] " << e.what() << std::endl;
            }
        }
    }
    return 0;
}
