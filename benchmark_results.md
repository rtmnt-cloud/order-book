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

## Next target
Replace `std::map` (red-black tree, pointer chasing) with a flat array indexed by price tick.
Expected gain: significant — map lookup is O(log N) with cache misses on every node traversal.
