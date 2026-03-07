import os
import zmq
import json
import asyncio
from dotenv import load_dotenv
from alpaca.data.live import NewsDataStream
from transformers import AutoTokenizer, AutoModelForSequenceClassification
import torch

load_dotenv()

# ZeroMQ Setup
context = zmq.Context()
zmq_socket = context.socket(zmq.PUB)
zmq_socket.bind("tcp://*:5555")

# 1. Load FinBERT Model and Tokenizer (This may take a minute on first run)
print("Loading FinBERT model from Hugging Face...")
tokenizer = AutoTokenizer.from_pretrained("ProsusAI/finbert")
model = AutoModelForSequenceClassification.from_pretrained("ProsusAI/finbert")

async def news_handler(data):
    headline = data.headline
    
    # 2. Perform Inference (AI Analysis)
    inputs = tokenizer(headline, return_tensors="pt", padding=True, truncation=True)
    with torch.no_grad():
        outputs = model(**inputs)
        
    # FinBERT outputs 3 classes: [Positive, Negative, Neutral]
    probabilities = torch.nn.functional.softmax(outputs.logits, dim=-1)
    
    # Calculate a single sentiment score (-1 to 1)
    # Score = Prob(Positive) - Prob(Negative)
    pos, neg, neu = probabilities[0].tolist()
    sentiment_score = round(pos - neg, 2)
    
    # Determine action based on score
    action = "NEUTRAL"
    if sentiment_score > 0.3: action = "BUY"
    elif sentiment_score < -0.3: action = "SELL"

    payload = {
        "ticker": data.symbols[0] if data.symbols else "GENERIC",
        "headline": headline,
        "sentiment_score": sentiment_score,
        "action": action
    }
    
    zmq_socket.send_string(f"SIGNAL {json.dumps(payload)}")
    print(f"[AI SIGNAL] {payload['ticker']} | Score: {sentiment_score} | {action}")

async def main():
    stream = NewsDataStream(os.getenv("ALPACA_API_KEY"), os.getenv("ALPACA_SECRET_KEY"))
    stream.subscribe_news(news_handler, "*")
    print("Alpaca + FinBERT Pipeline Active.")
    await stream._run_forever()

if __name__ == "__main__":
    asyncio.run(main())
