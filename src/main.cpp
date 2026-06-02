#include "PriceEngine.hpp"
#include "OrderBook.hpp"
#include<iostream>

int main(){
    PriceEngine engine(100.0, 0.5);
    OrderBook book;
    
    book.addOrder({OrderType::Buy, 101.00, 10, "Alice"});
    book.addOrder({OrderType::Buy, 99.50, 5, "Alice"});
    book.addOrder({OrderType::Sell, 100.50, 7, "Bob"});
    book.addOrder({OrderType::Sell, 102.00, 3, "Bob"});

    book.printBook();
    book.matchOrders();
    book.printBook(); //print after matching

    engine.run();
    return 0;
}