#pragma once
#include <vector>
#include "OrderBook.hpp"
#include "Trader.hpp"
#include "PriceQueue.hpp"
using namespace std;


class Algorithm {
public: 
    Algorithm(Trader &trader, OrderBook& book, int smaPeriod, double threshold,
        PriceQueue& queue);

    void run();
    void onPrice(double price);
    void printStats() const;

private:
    Trader& trader_;
    OrderBook& book_;

    int smaPeriod_;
    double threshold_;
    vector<double> prices_;
    PriceQueue& queue_;

    double calcSMA() const;

};

