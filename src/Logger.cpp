#include "Logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string& priceFile, const std::string& tradeFile) {
    priceLog_.open(priceFile);
    tradeLog_.open(tradeFile);

    if (!priceLog_.is_open() || !tradeLog_.is_open()) {
        std::cerr << "[LOGGER] Failed to open log files\n";
        return;
    }

    // CSV headers
    priceLog_ << "timestamp,price\n";
    tradeLog_ << "timestamp,buyer,seller,price,quantity\n";

    std::cout << "[LOGGER] Logging prices to " << priceFile
              << ", trades to " << tradeFile << "\n";
}

Logger::~Logger() {
    if (priceLog_.is_open()) priceLog_.close();
    if (tradeLog_.is_open()) tradeLog_.close();
}

void Logger::logPrice(double price) {
    std::lock_guard<std::mutex> lock(mutex_);
    priceLog_ << std::fixed << std::setprecision(2)
              << timestamp() << "," << price << "\n";
    priceLog_.flush();
}

void Logger::logTrade(const std::string& buyer, const std::string& seller,
                      double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex_);
    tradeLog_ << std::fixed << std::setprecision(2)
              << timestamp() << ","
              << buyer << ","
              << seller << ","
              << price << ","
              << quantity << "\n";
    tradeLog_.flush();
}

std::string Logger::timestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}