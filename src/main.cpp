#include "PriceEngine.hpp"
#include "OrderBook.hpp"
#include "Trader.hpp"
#include<iostream>

int main(){
    PriceEngine engine(100.0, 0.5);
    OrderBook book;

    Trader alice("Alice", 10000.0);
    Trader bob("Bob", 10000.0);

    // Bob lists some shares for sale
    book.addOrder({OrderType::Sell, 100.50, 7, "Bob"});
    book.addOrder({OrderType::Sell, 102.00, 3, "Bob"});
    
    // Alice buys - triggers matching automatically
    alice.buy(book,101.00,10);

    // Show portfolios and order book before matching
    double currentPrice = engine.getPrice();
    alice.printPortfolio(currentPrice);
    bob.printPortfolio(currentPrice);



    engine.run();
    return 0;
}