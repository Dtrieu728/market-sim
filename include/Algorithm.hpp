#pragma once
#include <vector>
#include "OrderBook.hpp"
#include "Trader.hpp"
#include "PriceQueue.hpp"
using namespace std;


class Algorithm {
public: 
    Algorithm(Trader &trader, OrderBook& book,PriceQueue& queue);
    virtual ~Algorithm() = default;

    void run();
    virtual void onPrice(double price) = 0;
    void printStats() const;

protected:
    Trader& trader_;
    OrderBook& book_;
    PriceQueue& queue_;
    vector<double> prices_;
   
};

