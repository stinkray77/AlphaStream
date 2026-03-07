import zmq
import json
import asyncio
from alpaca.data.live import NewsDataStream
import os
from dotenv import load_dotenv

load_dotenv() # This loads the variables from your .env file

API_KEY = os.getenv("ALPACA_API_KEY")
SECRET_KEY = os.getenv("ALPACA_SECRET_KEY")

print(f"Attempting Auth with Key: {API_KEY[:5]}*****") # Print first 5 chars to verify load

# ZeroMQ Setup
context = zmq.Context()
zmq_socket = context.socket(zmq.PUB)
zmq_socket.bind("tcp://*:5555")


async def news_handler(data):
    """
    This function runs every time a new headline arrives via the websocket.
    """
    # For now, we will pass the headline to ZeroMQ. 
    # In the next phase, we'll put the Hugging Face model here to analyze it.
    payload = {
        "ticker": data.symbols[0] if data.symbols else "GENERIC",
        "headline": data.headline,
        "sentiment_score": 0.0, # Placeholder until Phase 7
        "action": "NEUTRAL"    # Placeholder until Phase 7
    }
    
    message = f"SIGNAL {json.dumps(payload)}"
    zmq_socket.send_string(message)
    print(f"[LIVE NEWS] Broadcasted: {data.headline}")

async def main():

    # Initialize the Alpaca News Stream
    stream = NewsDataStream(
        api_key=os.getenv("ALPACA_API_KEY"), 
        secret_key=os.getenv("ALPACA_SECRET_KEY")
    )
    
    # Subscribe to all news (or specific symbols like ["AAPL", "TSLA"])
    stream.subscribe_news(news_handler, "*")
    
    print("Alpaca Websocket started. Listening for live news...")
    await stream._run_forever()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Stream stopped.")
