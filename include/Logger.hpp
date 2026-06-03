#pragma once
#include<string>
#include<fstream>
#include<mutex>


class Logger {
public:
    Logger(const std::string& priceFile, const std::string& tradeFile);
    ~Logger();
    

    void logPrice(double price);
    void logTrade(const std::string& buyer, const std::string& seller,
        double price, int quantitiy);

private:
        std::ofstream priceLog_;
        std::ofstream tradeLog_;
        std::mutex mutex_;

        std::string timestamp() const;
};