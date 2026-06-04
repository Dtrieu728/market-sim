#pragma once
#include<atomic>
#include "Logger.hpp"
#include "PriceQueue.hpp"
using namespace std;

class PriceEngine {
public:
    PriceEngine(double startPrice, double volatility, PriceQueue& queue, Logger& logger);
    // runs on its own thread - loops until stop
    void run();
    void stop();
    void tick();
    void loadCSV(const string& filename);
    double getPrice() const;
    

private:
    double price_;
    double volatility_;
    atomic<bool> running_;
    PriceQueue& queue_;
    Logger& logger_;

    vector<double> replayPrices_;
    bool replayMode_ = false;

    double nextPrice();
    
};