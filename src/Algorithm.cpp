#include "Algorithm.hpp"
#include <numeric>
#include <iostream>
#include <iomanip>
using namespace std;
/*
Algorithm class that receives a price on every tick 
and decides whether to buy or sell based on the Simple Moving Average (SMA) of the last N prices.
simple moving average (SMA) is calculated by taking the average of the last N prices.
The algorithm compares the current price to the SMA and if the price is below the SMA by a
*/
Algorithm::Algorithm(Trader &trader, OrderBook& book, PriceQueue& queue)
    : trader_(trader), book_(book), queue_(queue) {}


void Algorithm::run() {
    while (true) {
        auto price = queue_.pop();
        if (!price.has_value()) {
            if (queue_.isClosed()) break;   // queue closed — exit
            continue;                        // just a timeout — keep waiting
        }
        onPrice(price.value());
    }
}


void Algorithm::printStats() const {
    cout << "\n[ALGO] Total prices observed: " << prices_.size() << "\n";
}


