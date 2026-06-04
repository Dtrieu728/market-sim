#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
using namespace std;


class PriceQueue{
public:
    void push(double price);
    optional<double> pop(int timeoutMs = 500);
    void close();
    bool isClosed() const;
private:
    queue<double> queue_;
    mutex mutex_;
    condition_variable cv_;
    bool closed_ = false;
};

