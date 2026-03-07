import zmq, time
ctx = zmq.Context()
sock = ctx.socket(zmq.PUB)
sock.bind("tcp://*:5555")
while True:
    print("Sending...")
    sock.send_string("SIGNAL {}") # Sending empty JSON
    time.sleep(1)
