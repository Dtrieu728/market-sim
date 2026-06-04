#include "PriceQueue.hpp"
using namespace std;

void PriceQueue::push(double price){
    {
    lock_guard<mutex> lock(mutex_);
    queue_.push(price);
    }
    cv_.notify_one();
}

optional<double> PriceQueue::pop(int timeoutMs){
    unique_lock<mutex> lock(mutex_);
    cv_.wait_for(
        lock,
        chrono::milliseconds(timeoutMs),
        [this]{ return !queue_.empty() || closed_;} 
    );

    if(queue_.empty()) return nullopt;

    double val = queue_.front();
    queue_.pop();
    return val;
};


void PriceQueue::close(){
    {
    lock_guard<mutex> lock(mutex_);
    closed_ = true;
    }
    cv_.notify_all();
}

bool PriceQueue::isClosed() const {
    return closed_;
}