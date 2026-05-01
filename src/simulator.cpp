#include "simulator.h"
#include <iostream>
#include <thread>



Simulator::Simulator(): totalOrders(0), totalTrades(0), midPrice(0){}


void Simulator::run( int numOrders )
{
    std::atomic<bool> done{false};
    std::thread producer ( [&]() {midPrice = 100;
    for ( int i = 0 ; i < numOrders ; i++)
    {
        Order order;
        order.orderId = i;
        order.traderId = rand() % 1000;
        order.instrumentId = rand() % 5;
        order.side = (i % 2 == 0) ? Side::Buy : Side::Sell;
        order.price = rand() % 20 + 90;
        order.quantity = rand() % 500 + 10;
        order.timestamp = std::chrono::steady_clock::now();

        while(!queue.push(order));
    } done.store(true);

});
    std::thread consumer ( [&]() 
    {
        Order order;
        while( !done.load()|| !queue.empty())
        {
            if (queue.pop(order))
            engine.process_order(order);

        }




    });

    producer.join();
    consumer.join();
    totalOrders = numOrders;
    totalTrades = engine.get_trades().size();

}
void Simulator::printStats ( ) const
{

    std::cout << " Total orders : " << totalOrders <<"\n";
    std::cout << " Total trades : " << totalTrades <<"\n";
    std::cout << " Fill rate : "<< 100 * totalTrades/totalOrders ;


}