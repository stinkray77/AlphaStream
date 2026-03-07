import zmq
import json
import time

def start_publisher():
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    
    # Bind to port 5555. Using TCP allows Docker containers to communicate.
    socket.bind("tcp://*:5555")
    
    print("ZeroMQ Publisher bound to port 5555. Waiting for subscribers...")
    time.sleep(1) # Brief pause to allow subscribers to connect

    # Simulated output from your Hugging Face NLP model
    signal_payload = {
        "ticker": "AAPL",
        "sentiment_score": 0.85,
        "action": "BUY"
    }
    
    # ZeroMQ PUB-SUB uses topic filtering. We prefix the message with the topic "SIGNAL"
    message = f"SIGNAL {json.dumps(signal_payload)}"
    
    while True:
        socket.send_string(message)
        print(f"Broadcasted: {message}")
        time.sleep(2) # Simulating a stream of incoming news

if __name__ == "__main__":
    start_publisher()
