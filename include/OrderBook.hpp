#pragma once
#include<vector>
#include<string>
#include"Logger.hpp"
using namespace std;

// stores Buy Order(bids) & Sell Orders(asks) : P_Buy >= P_ask
enum class OrderType {Buy, Sell};

struct Order{
    OrderType type;
    double price;
    int quantity;
    string trader; //who placed it; used for portfolio tracking later
};


class OrderBook{
public:
    OrderBook(Logger& logger);
    void addOrder(const Order& order);
    void matchOrders();
    void printBook() const;


private:
    vector<Order> bids_;
    vector<Order> asks_;
    Logger& logger_;
};


