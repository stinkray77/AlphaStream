#include <zmq.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

// Create an alias for convenience
using json = nlohmann::json;

// Define the strictly typed structure for your Limit Order Book
struct AlphaSignal
{
    std::string ticker;
    double sentiment_score;
    std::string action; // "BUY" or "SELL"
};

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::sub);

    std::cout << "Connecting to AlphaStream Inference publisher..." << std::endl;
    socket.connect("tcp://localhost:5555");
    socket.set(zmq::sockopt::subscribe, "SIGNAL");

    while (true)
    {
        zmq::message_t update;
        auto res = socket.recv(update, zmq::recv_flags::none);

        if (res)
        {
            std::string msg_str = update.to_string();
            std::string json_payload = msg_str.substr(7); // Strip "SIGNAL "

            try
            {
                // 1. Parse the raw string into a JSON object
                json j = json::parse(json_payload);

                // 2. Map the JSON object to our C++ struct
                AlphaSignal signal;
                signal.ticker = j["ticker"];
                signal.sentiment_score = j["sentiment_score"];
                signal.action = j["action"];

                // 3. Verify it is a structured C++ object by accessing members directly
                std::cout << "\n--- Parsed Signal Struct ---" << std::endl;
                std::cout << "Target Asset: " << signal.ticker << std::endl;
                std::cout << "Confidence:   " << signal.sentiment_score << std::endl;
                std::cout << "Execution:    " << signal.action << std::endl;
            }
            catch (json::parse_error &e)
            {
                std::cerr << "JSON Parsing Error: " << e.what() << '\n';
            }
            catch (json::type_error &e)
            {
                std::cerr << "JSON Type Mapping Error (Missing/Wrong Fields): " << e.what() << '\n';
            }
        }
    }
    return 0;
}
