#include <gtest/gtest.h>
#include "matching_engine.h"

static Order makeOrder(uint64_t id, uint64_t traderId, Side side, double price, uint32_t qty) {
    return {id, traderId, 1001, side, price, qty, std::chrono::steady_clock::now()};
}

// Full match: buy qty == sell qty
TEST(MatchingEngine, FullMatch) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 101, Side::Buy,  100.0, 50));
    engine.process_order(makeOrder(2, 102, Side::Sell, 100.0, 50));

    const auto& trades = engine.get_trades();
    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].quantity, 50u);
    EXPECT_DOUBLE_EQ(trades[0].price, 100.0);
    EXPECT_EQ(trades[0].buyerId,  101u);
    EXPECT_EQ(trades[0].sellerId, 102u);
}

// Partial fill: sell qty < buy qty, remainder stays in book
TEST(MatchingEngine, PartialFill) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 101, Side::Buy,  100.0, 100));
    engine.process_order(makeOrder(2, 102, Side::Sell, 100.0,  40));

    const auto& trades = engine.get_trades();
    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].quantity, 40u);
}

// No match: sell price above best bid
TEST(MatchingEngine, NoMatchPriceTooHigh) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 101, Side::Buy,  100.0, 50));
    engine.process_order(makeOrder(2, 102, Side::Sell, 101.0, 50));

    EXPECT_EQ(engine.get_trades().size(), 0u);
}

// Incoming order matches multiple resting levels
TEST(MatchingEngine, MatchMultipleLevels) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 101, Side::Buy, 101.0, 30));
    engine.process_order(makeOrder(2, 102, Side::Buy, 100.0, 30));
    engine.process_order(makeOrder(3, 103, Side::Sell, 99.0, 50));

    const auto& trades = engine.get_trades();
    ASSERT_EQ(trades.size(), 2u);
    EXPECT_EQ(trades[0].quantity, 30u);
    EXPECT_EQ(trades[1].quantity, 20u);
}

// Buyer/seller IDs are correct for sell-side aggressor
TEST(MatchingEngine, BuyerSellerIds_SellAggressor) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 201, Side::Buy,  100.0, 10));
    engine.process_order(makeOrder(2, 202, Side::Sell,  99.0, 10));

    const auto& trades = engine.get_trades();
    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buyerId,  201u);
    EXPECT_EQ(trades[0].sellerId, 202u);
}

// Buyer/seller IDs are correct for buy-side aggressor
TEST(MatchingEngine, BuyerSellerIds_BuyAggressor) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 201, Side::Sell, 100.0, 10));
    engine.process_order(makeOrder(2, 202, Side::Buy,  101.0, 10));

    const auto& trades = engine.get_trades();
    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buyerId,  202u);
    EXPECT_EQ(trades[0].sellerId, 201u);
}

// Resting order fully consumed, should be removed from book
TEST(MatchingEngine, RestingOrderRemovedAfterFullFill) {
    MatchingEngine engine;
    engine.process_order(makeOrder(1, 101, Side::Buy,  100.0, 10));
    engine.process_order(makeOrder(2, 102, Side::Sell, 100.0, 10));
    engine.process_order(makeOrder(3, 103, Side::Sell, 100.0, 10));

    // Order 1 is fully consumed by order 2; order 3 should not match anything
    EXPECT_EQ(engine.get_trades().size(), 1u);
}
