# test_signal.py (Updated)
import zmq, json, time

ctx = zmq.Context()
sock = ctx.socket(zmq.PUB)
sock.bind("tcp://*:5555") # Use * to bind to all interfaces

print("Test Source active on :5555. Waiting for C++ engine...")
time.sleep(2.0) # GIVE THE C++ ENGINE TIME TO CONNECT

payload = {
    "ticker": "NVDA",
    "sentiment_score": 0.88,
    "action": "BUY"
}

message = f"SIGNAL {json.dumps(payload)}"
sock.send_string(message)
print(f"Dispatched: {message}")
