#include "OrderBook.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>

OrderBook::OrderBook(Logger& logger) : logger_(logger){}

void OrderBook::addOrder(const Order& order){
    if(order.type == OrderType::Buy){
        bids_.push_back(order); 
        std::sort(bids_.begin(), bids_.end(), [](const Order& a, const Order& b){
            return a.price > b.price; // Higher price first
        });

    }else{
        asks_.push_back(order);
        std::sort(asks_.begin(), asks_.end(), [](const Order& a, const Order& b){
            return a.price < b.price; // Lower price first
        });
    }
}


void OrderBook::matchOrders() {
    /*compute best bid vs best ask
    if match -> execute trade
    Reduce quantities
    Remove filled orders
    */
    while (!bids_.empty() && !asks_.empty()) {
        Order& bestBid = bids_.front();
        Order& bestAsk = asks_.front();

        if (bestBid.price >= bestAsk.price) {
            int filled = std::min(bestBid.quantity, bestAsk.quantity);
            double fillPrice = bestAsk.price;   // convention: fill at ask price

            std::cout << std::fixed << std::setprecision(2)
                      << "[TRADE] " << bestBid.trader << " bought "
                      << filled << " @ $" << fillPrice
                      << " from " << bestAsk.trader << "\n";

            logger_.logTrade(bestBid.trader, bestAsk.trader,fillPrice,filled);
            bestBid.quantity -= filled;
            bestAsk.quantity -= filled;

            if (bestBid.quantity == 0) bids_.erase(bids_.begin());
            if (bestAsk.quantity == 0) asks_.erase(asks_.begin());
        } else {
            break;   // no match possible — best bid < best ask
        }
    }
}

void OrderBook::printBook() const {
    std::cout << "\n--- Order Book ---\n";
    std::cout << "ASKS:\n";
    for (const auto& o : asks_)
        std::cout << "  $" << std::fixed << std::setprecision(2)
                  << o.price << "  qty:" << o.quantity
                  << "  (" << o.trader << ")\n";
    std::cout << "BIDS:\n";
    for (const auto& o : bids_)
        std::cout << "  $" << std::fixed << std::setprecision(2)
                  << o.price << "  qty:" << o.quantity
                  << "  (" << o.trader << ")\n";
    std::cout << "------------------\n\n";
}