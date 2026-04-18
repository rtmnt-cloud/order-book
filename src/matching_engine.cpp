#include "matching_engine.h"

void MatchingEngine::process_order(Order order)
{   


   

    //consume matching order
    while( order.quantity > 0)
    {
        //see if matching oder exists
        std::optional<Order> bestMatch;
        if ( order.side == Side::Sell)
        bestMatch = orderBook.get_best_bid();
        else
        bestMatch = orderBook.get_best_ask();
        if (!bestMatch.has_value()) break;
        if ( bestMatch->side == Side::Sell && bestMatch->price > order.price) break;
        if ( bestMatch->side == Side::Buy && bestMatch->price < order.price) break;

        unit32_t fillQty = std::min(bestMatch->quantity, order.quantity);
        // uint64_t buyer_id;
        // uint64_t seller_id;
        // uint64_t instrument_id;
        // double price;
        // uint32_t quantity;
        // std::chrono::steady_clock::time_point timestamp;
        Trade trade = { order.taderId, bestMatch->traderId, order.instrumentId,
                        bestMatch->price, fillQty,std::chrono::steady_clock::now()} }
        trades.emplace_back(trade);
        
        bestMatch.quantity -= fillQty;
        order.quantity -= fillQty;
    //delete order if exhausted

    if ( bestMatch->quantity == 0)
    {
        orderBook.cancel_order(bestMatch->orderId);

    }

    //add order to orderBook is not exhausted
    if ( order.quantity > 0)
    {

        orderBook.add_order(order);

    }
}

const std::vector<Trade>& MatchingEngine::get_trades() const
{
}
