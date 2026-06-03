#pragma once
#include<atomic>

class PriceEngine {
public:
    PriceEngine(double startPrice, double volatility);
    void run();
    void stop();
    void tick();
    double getPrice() const;
    

private:
    double price_;
    double volatility_;
    std::atomic<bool> running_;

    double nextPrice();
    
};