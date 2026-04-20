# Benchmark Results
## Setup
- 1,000,000 orders per run
- Alternating Buy/Sell, random prices (0-99), random qty (1-100)
- Seeded with srand(42) for reproducibility
- Measured with std::chrono::steady_clock

---

## Baseline — std::list + std::map (heap-allocated)
**Data structures:** `std::map<double, std::list<Order>>` for bids/asks, list iterators in orderMap

50-run results:
- Range: 751–821 ns/order
- Average: ~782 ns/order
- Throughput: ~1.28M orders/sec

---

## Layer 4 — Pool-backed intrusive linked list
**What changed:** Replaced `std::list<Order>` with a pre-allocated `MemoryPool` (65536 slots).
Each `Order` has `next`/`prev` indices forming an intrusive doubly-linked list per price level.
`orderMap` now stores pool indices instead of list iterators.
Zero heap allocations on the hot path.

50-run results:
- Range: 529–641 ns/order
- Average: ~558 ns/order
- Throughput: ~1.79M orders/sec

**Improvement: ~29% faster, +500K orders/sec**

---

## Layer 4b — Flat price array + O(1) best bid/ask tracker
**What changed:** Replaced `std::map<double, PriceLevel>` with `PriceLevel bids[1024]` / `PriceLevel asks[1024]`.
Added `bestBid` and `bestAsk` integer trackers updated on every add/cancel.
`get_best_bid()` and `get_best_ask()` are now O(1) direct array lookups.

50-run results:
- Range: 367–424 ns/order
- Average: ~388 ns/order
- Throughput: ~2.58M orders/sec

**Improvement over memory pool baseline: ~30% faster, +790K orders/sec**
**Improvement over original baseline: ~50% faster, +1.3M orders/sec**

---

## Next target
Layer 5: Simulator — generate realistic random order flow, stress test the engine.
