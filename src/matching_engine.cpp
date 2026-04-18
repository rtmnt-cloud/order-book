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


        
    }
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
