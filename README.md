# AlphaStream: Real-Time AI Sentiment Trading Pipeline

AlphaStream is a distributed trading infrastructure that bridges high-level AI inference with low-latency execution. It ingests live financial news, performs sentiment analysis using a transformer model, and routes trade signals to a high-performance C++ matching engine.


## 🏗️ System Architecture
- **Inference Service (Python):** Subscribes to Alpaca's real-time news WebSocket. It utilizes **FinBERT** (Hugging Face) to generate sentiment scores (-1.0 to 1.0) and translates them into trade actions (BUY/SELL/NEUTRAL).
- **Execution Engine (C++):** A high-performance service hosting a **Limit Order Book (LOB)**. It listens for signals via ZeroMQ and executes trades using a price-time priority matching algorithm.
- **Communication Layer (ZeroMQ):** Uses a PUB/SUB pattern to decouple the high-latency AI layer from the low-latency execution layer.

## 🚀 Technical Highlights & Challenges
- **Latency Optimization:** Implemented **ZeroMQ** to ensure asynchronous communication, preventing the AI inference overhead from blocking matching engine operations.
- **Cross-Platform Compatibility:** Resolved **GLIBC ABI versioning conflicts** and library linking issues (`nlohmann_json`, `libzmq`) by engineering a multi-stage Docker build process with unified Debian bases.
- **Defensive Engineering:** Developed robust JSON parsing in C++ that utilizes pattern matching (`find('{')`) and key validation to ensure pipeline stability against malformed network packets.
- **ML Performance:** Leveraged **Hugging Face Accelerate** and **MPS (Metal Performance Shaders)** to optimize transformer inference on Apple Silicon hardware.

## 🛠️ How to Run
1. Clone the repository.
2. Add your `ALPACA_API_KEY` and `ALPACA_SECRET_KEY` to `python_inference_service/.env`.
3. Run the orchestrated environment:
   ```bash
   docker compose up --build
