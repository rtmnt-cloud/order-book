# Order Book Project Context

## What this project is
A low-latency order book and matching engine in C++, built as an HFT portfolio project.

## Architecture
- OrderBook: stores resting orders (DONE)
- MatchingEngine: matches incoming orders, generates trades (IN PROGRESS)
- Trade: struct representing an executed transaction
- Simulator: generates random orders to stress test (TODO)

## Layer progress
- Layer 1 (OrderBook): DONE and working
- Layer 2 (MatchingEngine): IN PROGRESS
- Layer 3-6: TODO

## Current task
Implementing MatchingEngine class. It owns an OrderBook, takes Orders as input via process_order(), and produces Trade records as output.

## Key files
- include/order.h - Order struct (needs trader_id and instrument_id added)
- include/order_book.h - OrderBook class
- include/trade.h - Trade struct (done)
- include/matching_engine.h - TODO
- src/order_book.cpp - OrderBook implementation
