#pragma once
#include <vector>
#include "OrderBook.hpp"
#include "Trader.hpp"


class Algorithm {
public: 
    Algorithm(Trader &trader, OrderBook& book, int smaPeriod, double threshold);

    void onPrice(double price);
    void printStats() const;

private:
    Trader& trader_;
    OrderBook& book_;

    int smaPeriod_;
    double threshold_;
    std::vector<double> prices_;

    double calcSMA() const;

};

