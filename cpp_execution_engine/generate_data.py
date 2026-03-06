import csv
import random

# Configuration
NUM_ORDERS = 1000000  # 1 Million orders
OUTPUT_FILE = "big_market_data.csv"

print(f"Generating {NUM_ORDERS} orders...")

with open(OUTPUT_FILE, "w", newline="") as f:
    writer = csv.writer(f)
    # No header row for simplicity in C++
    
    for i in range(NUM_ORDERS):
        # 1. ID: Simple counter
        order_id = i + 1
        
        # 2. Side: True (Buy) or False (Sell)
        is_buy = "1" if random.random() > 0.5 else "0"
        
        # 3. Price: Random between $100.00 and $200.00 (in cents)
        price = random.randint(10000, 20000)
        
        # 4. Quantity: Random between 1 and 100
        qty = random.randint(1, 100)
        
        writer.writerow([order_id, is_buy, price, qty])

print(f"Done! Created {OUTPUT_FILE}")