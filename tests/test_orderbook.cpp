#include <cassert>
#include <iostream>
#include "OrderBook.hpp"
#include "Logger.hpp"

void testBasicMatch() {
    Logger logger("test_prices.csv", "test_trades.csv");
    OrderBook book(logger);

    book.addOrder({OrderType::Buy,  101.00, 5, "Buyer"});
    book.addOrder({OrderType::Sell, 100.00, 5, "Seller"});
    book.matchOrders();

    std::cout << "[TEST] testBasicMatch passed\n";
}

void testNoMatch() {
    Logger logger("test_prices2.csv", "test_trades2.csv");
    OrderBook book(logger);

    // bid below ask — should not match
    book.addOrder({OrderType::Buy,  99.00, 5, "Buyer"});
    book.addOrder({OrderType::Sell, 101.00, 5, "Seller"});
    book.matchOrders();

    std::cout << "[TEST] testNoMatch passed\n";
}

void testPartialFill() {
    Logger logger("test_prices3.csv", "test_trades3.csv");
    OrderBook book(logger);

    book.addOrder({OrderType::Buy,  101.00, 10, "Buyer"});
    book.addOrder({OrderType::Sell, 100.00,  4, "Seller"});
    book.matchOrders();

    std::cout << "[TEST] testPartialFill passed\n";
}

int main() {
    testBasicMatch();
    testNoMatch();
    testPartialFill();
    std::cout << "[TEST] All tests passed\n";
    return 0;
}