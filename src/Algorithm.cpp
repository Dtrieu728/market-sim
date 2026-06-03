#include "Algorithm.hpp"
#include <numeric>
#include <iostream>
#include <iomanip>

/*
Algorithm class that receives a price on every tick 
and decides whether to buy or sell based on the Simple Moving Average (SMA) of the last N prices.
simple moving average (SMA) is calculated by taking the average of the last N prices.
The algorithm compares the current price to the SMA and if the price is below the SMA by a
*/
Algorithm::Algorithm(Trader &trader, OrderBook& book, int smaPeriod, double threshold)
    : trader_(trader), book_(book), smaPeriod_(smaPeriod), threshold_(threshold) {}


double Algorithm::calcSMA() const{
    if (prices_.size() < static_cast<size_t>(smaPeriod_)) {
        return 0.0; // Not enough data to calculate SMA
    }
    double sum = std::accumulate(prices_.end() - smaPeriod_, prices_.end(), 0.0);
    return sum / smaPeriod_;
}

void Algorithm::onPrice(double price){
    prices_.push_back(price);

    if(prices_.size() < static_cast<size_t>(smaPeriod_)) {
        std::cout << "[ALGO] Warming up... ("
                  << prices_.size() << "/" << smaPeriod_ << ")\n";
        return; // Not enough data to make a decision
    }

    double sma = calcSMA();
    double diff = (price - sma) /sma;
    std::cout << std::fixed << std::setprecision(2)
              << "[ALGO] Price: $" << price
              << "  SMA(" << smaPeriod_ << "): $" << sma
              << "  Diff: " << std::setprecision(3) << diff * 100 << "%\n";
   if (diff < -threshold_ && trader_.getCash() >= price) {
        // price is below SMA by threshold_ — buy signal
        std::cout << "[ALGO] BUY signal\n";
        trader_.buy(book_, price, 1);
        trader_.printPortfolio(price);

    } else if (diff > threshold_ && trader_.getHoldings() > 0) {
        // price is above SMA by threshold_ — sell signal
        std::cout << "[ALGO] SELL signal\n";
        trader_.sell(book_, price, 1);
        trader_.printPortfolio(price);
    }
}

void Algorithm::printStats() const {
    std::cout << "\n[ALGO] Total prices observed: " << prices_.size() << "\n";
}