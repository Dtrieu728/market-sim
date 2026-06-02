#include "Trader.hpp"
#include <iostream>
#include <iomanip>

Trader::Trader(const std::string& name, double cash)
    : name_(name), cash_(cash), holdings_(0), avgCost_(0.0) {}


void Trader::buy(OrderBook& book, double price, int quantity) {
    double cost = price * quantity;
    if (cost > cash_) {
        std::cout << "[" << name_ << "] Not enough cash to buy "
                  << quantity << " @ $" << price << "\n";
        return;
    }

    double totalCost = avgCost_ * holdings_ + cost;
    holdings_ += quantity;
    avgCost_ = (holdings_ > 0) ? totalCost / holdings_ : 0.0;
    cash_ -= cost;

    book.addOrder({OrderType::Buy, price, quantity, name_});
    book.matchOrders();
}


void Trader::sell(OrderBook& book, double price, int quantity) {
    if (quantity > holdings_) {
        std::cout << "[" << name_ << "] Not enough holdings to sell "
                  << quantity << "\n";
        return;
    }
    holdings_ -= quantity;
    cash_ += price * quantity;

    book.addOrder({OrderType::Sell, price, quantity, name_});
    book.matchOrders();
}

void Trader::updatePnL(double currentPrice) {
    double unrealized = (currentPrice - avgCost_) * holdings_;
     std::cout << std::fixed << std::setprecision(2)
              << "[" << name_ << "] P&L  unrealized: $" << unrealized
              << "  cash: $" << cash_
              << "  holdings: " << holdings_
              << "  avg cost: $" << avgCost_ << "\n";
}


void Trader::printPortfolio(double currentPrice) const {
    double marketValue = currentPrice * holdings_;
    double unrealised  = (currentPrice - avgCost_) * holdings_;
    double totalValue  = cash_ + marketValue;

    std::cout << "\n=== Portfolio: " << name_ << " ===\n"
              << std::fixed << std::setprecision(2)
              << "  Cash:         $" << cash_ << "\n"
              << "  Holdings:      " << holdings_ << " units\n"
              << "  Avg cost:     $" << avgCost_ << "\n"
              << "  Market value: $" << marketValue << "\n"
              << "  Unrealised:   $" << unrealised << "\n"
              << "  Total value:  $" << totalValue << "\n"
              << "========================\n\n";
}

const std::string& Trader::getName() const { return name_; }
double Trader::getCash() const { return cash_; }
int Trader::getHoldings() const { return holdings_; }