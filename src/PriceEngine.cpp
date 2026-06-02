#include "PriceEngine.hpp"; 
#include<iostream>
#include<thread>
#include<chrono>
#include<iomanip>
#include<random>

PriceEngine::PriceEngine(double startPrice, double volatility) 
    : price_(startPrice), volatility_(volatility), running_(false) {}


double PriceEngine::nextPrice() {
    static std::mt19937 rng(std::random_device{}());
    static std::normal_distribution<double> dist(0.0,1.0);
    return price_ + volatility_ * dist(rng);
}


void PriceEngine::run(){
    running_ = true;
    while(running_){
        price_ = nextPrice();
        std::cout<< std::fixed << std::setprecision(2) 
            << "[PRICE] $ " << price_ << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        

    }
}

void PriceEngine::stop(){
    running_ = false;
}

double PriceEngine::getPrice() const{
    return price_;
}

