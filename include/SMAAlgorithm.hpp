#pragma once
#include "Algorithm.hpp"

class SMAAlgorithm : public Algorithm {
public:
    SMAAlgorithm(Trader& trader, OrderBook& book, PriceQueue& queue,
                 int smaPeriod, double threshold);

    void onPrice(double price) override;

private:
    int smaPeriod_;
    double threshold_;

    double calcSMA() const;
};