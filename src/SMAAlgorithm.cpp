#include "SMAAlgorithm.hpp"
#include <iostream>
#include <numeric>
#include <iomanip>
using namespace std;

SMAAlgorithm::SMAAlgorithm(Trader& trader, OrderBook& book, PriceQueue& queue,
                             int smaPeriod, double threshold)
    : Algorithm(trader, book, queue),
      smaPeriod_(smaPeriod), threshold_(threshold) {}

double SMAAlgorithm::calcSMA() const {
    if (prices_.size() < static_cast<size_t>(smaPeriod_)) return 0.0;
    double sum = accumulate(prices_.end() - smaPeriod_, prices_.end(), 0.0);
    return sum / smaPeriod_;
}

void SMAAlgorithm::onPrice(double price) {
    prices_.push_back(price);

    if (prices_.size() < static_cast<size_t>(smaPeriod_)) {
        std::cout << "[SMA] Warming up... ("
                  << prices_.size() << "/" << smaPeriod_ << ")\n";
        return;
    }

    double sma = calcSMA();
    double diff = (price - sma) / sma;

    std::cout << std::fixed << std::setprecision(2)
              << "[SMA] Price: $" << price
              << "  SMA(" << smaPeriod_ << "): $" << sma
              << "  Diff: " << setprecision(3) << diff * 100 << "%\n";

    if (diff < -threshold_ && trader_.getCash() >= price && trader_.getHoldings() < 5) {
        cout << "[SMA] BUY signal\n";
        trader_.buy(book_, price, 1);
        trader_.printPortfolio(price);
    } else if (diff > threshold_ && trader_.getHoldings() > 0) {
        cout << "[SMA] SELL signal\n";
        trader_.sell(book_, price, 1);
        trader_.printPortfolio(price);
    }
}