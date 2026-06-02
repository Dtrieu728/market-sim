#pragma once
#include<vector>
#include<string>

// stores Buy Order(bids) & Sell Orders(asks) : P_Buy >= P_ask
enum class OrderType {Buy, Sell};

struct Order{
    OrderType type;
    double price;
    int quantity;
    std::string trader; //who placed it; used for portfolio tracking later
};


class OrderBook{
public:
    void addOrder(const Order& order);
    void matchOrders();
    void printBook() const;


private:
    std::vector<Order> bids_;
    std::vector<Order> asks_;
};


