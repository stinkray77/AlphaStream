import zmq
import json
import time
import random

def start_publisher():
    context = zmq.Context()
    # Create a Publisher socket
    socket = context.socket(zmq.PUB)
    
    # Bind to all interfaces on port 5555
    socket.bind("tcp://*:5555")
    
    print("Python Inference Publisher bound to port 5555.")
    print("Waiting 3 seconds for C++ subscribers to connect...")
    time.sleep(3) 

    tickers = ["AAPL", "MSFT", "TSLA", "NVDA"]
    
    print("Starting mock sentiment stream...")
    while True:
        # Simulate a random sentiment signal
        payload = {
            "ticker": random.choice(tickers),
            "sentiment_score": round(random.uniform(-1.0, 1.0), 2),
            "action": "BUY" if random.random() > 0.5 else "SELL"
        }
        
        # Prefix the topic "SIGNAL " so the C++ engine knows to process it
        message = f"SIGNAL {json.dumps(payload)}"
        
        # Broadcast the message
        socket.send_string(message)
        print(f"[PUBLISHED] {message}")
        
        # Stream a new signal every 1 second
        time.sleep(1)

if __name__ == "__main__":
    start_publisher()
