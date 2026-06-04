#include "PriceEngine.hpp"
#include<iostream>
#include <fstream>
#include<sstream>
#include<thread>
#include<chrono>
#include<iomanip>
#include<random>
#include<atomic>
using namespace std;

extern atomic<bool> g_running;

PriceEngine::PriceEngine(double startPrice, double volatility, PriceQueue& queue, Logger& logger) 
    : price_(startPrice), volatility_(volatility), running_(false),queue_(queue), logger_(logger) {}

double PriceEngine::nextPrice() {
    static mt19937 rng(random_device{}());
    static normal_distribution<double> dist(0.0,1.0);
    return price_ + volatility_ * dist(rng);
}


void PriceEngine::loadCSV(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"[Price Engine] Could not open CSV: " <<filename << "\n";
        return;
    }

    string line;
    getline(file, line); // skips header row

    while(getline(file, line)){
        stringstream ss(line);
        string token;

        getline(ss,token,','); //skip timestamp
        getline(ss,token,','); // read price

        try{
            replayPrices_.push_back(stod(token));
        }catch (...){
            //skip malformed rows
        }
    }

    if (!replayPrices_.empty()) {
        replayMode_ = true;
        price_ = replayPrices_.front();
        std::cout << "[PRICE ENGINE] Replay mode: loaded "
                  << replayPrices_.size() << " prices from " << filename << "\n";
    }
}


void PriceEngine::run() {
    running_ = true;
    size_t idx = 0;

    while (running_ && g_running) {
        if (replayMode_) {
            if (idx >= replayPrices_.size()) {
                std::cout << "[PRICE ENGINE] Replay complete\n";
                break;
            }
            price_ = replayPrices_[idx++];
        } else {
            price_ = nextPrice();
        }

        std::cout << std::fixed << std::setprecision(2)
                  << "[PRICE] $" << price_ << "\n";
        logger_.logPrice(price_);
        queue_.push(price_);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    queue_.close();
}
void PriceEngine::stop(){
    running_ = false;
}

double PriceEngine::getPrice() const{
    return price_;
}

void PriceEngine::tick() {
    price_ = nextPrice();
}
