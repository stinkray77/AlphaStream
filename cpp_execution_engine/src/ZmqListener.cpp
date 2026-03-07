#include <zmq.hpp>
#include <iostream>
#include <string>

int main()
{
    // Initialize the ZeroMQ context with 1 I/O thread
    zmq::context_t context(1);

    // Create a Subscriber socket
    zmq::socket_t socket(context, zmq::socket_type::sub);

    std::cout << "Connecting to AlphaStream Inference publisher..." << std::endl;
    // Connect to the Python publisher. (localhost for now, later the Docker service name)
    socket.connect("tcp://localhost:5555");

    // Subscribe ONLY to messages starting with "SIGNAL"
    socket.set(zmq::sockopt::subscribe, "SIGNAL");

    while (true)
    {
        zmq::message_t update;

        // Block until a message arrives
        auto res = socket.recv(update, zmq::recv_flags::none);

        if (res)
        {
            std::string msg_str = update.to_string();

            // Strip the "SIGNAL " topic prefix to isolate the JSON payload
            std::string json_payload = msg_str.substr(7);

            std::cout << "Received Signal for Execution Engine: " << json_payload << std::endl;

            // TODO: Parse the JSON and route to your std::map Limit Order Book
        }
    }
    return 0;
}
