#include "spsc_queue.h"
// #include <atomic>
// #include "order.h"

// static constexpr uint32_t CAPACITY = 1024;
// class SPSCQueue
// {

//     public:
//     bool push(const Order&);
//     bool pop(Order&);



//     private:
//         Order buffer[CAPACITY];
//         std::atomic<unit32_t> head{0}, tail{0};


// };



 bool SPSCQueue::push(const Order& order)
 {


    auto currHead = head.load();
    auto currTail = tail.load();

    if (( tail +1) % CAPACITY == head )
    return false;

    buffer[currTail] =  order;
    tail.store((currTail+1)%CAPACITY);

    return true;
  
    
 }

 bool SPSCQueue::pop(Order& out)
 {


     auto currHead = head.load();
    auto currTail = tail.load();
    if (currHead == currTail)  return false;

    out = buffer[currHead];
    head.store((currHead + 1) % CAPACITY);

    return true;


 }