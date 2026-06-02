#pragma once
#include <string>
#include "OrderBook.hpp"

class Trader {
public:
    Trader(const std::string& name, double cash);

    void buy(OrderBook& book, double price, int quantity);
    void sell(OrderBook& book, double price, int quantity);

    void updatePnL(double currentPrice);
    void printPortfolio(double currentPrice) const;

    const std::string& getName() const;
    double getCash() const;
    int getHoldings() const;

private:
    std::string name_;
    double cash_;
    int holdings_; // Number of shares held
    double avgCost_; // Average cost per share for the holdings
};