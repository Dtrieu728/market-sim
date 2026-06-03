#include "PriceEngine.hpp"
#include "OrderBook.hpp"
#include "Trader.hpp"
#include "Algorithm.hpp"
#include<iostream>
#include<thread>
#include<chrono>
#include <iomanip>



int main() {
    Logger logger("prices.csv","trades.csv");

    PriceEngine engine(100.0, 0.5);
    OrderBook book(logger);
    Trader algo_trader("AlgoTrader", 10000.0);

    // SMA period: 5 ticks, threshold: 0.2% deviation to trigger trade
    Algorithm algo(algo_trader, book, 5, 0.002);

    std::cout << "Starting market simulation...\n\n";

    // run price engine manually tick by tick so algo can react
    for (int i = 0; i < 30; ++i) {
        engine.tick();                          // generate next price
        double price = engine.getPrice();
        std::cout << std::fixed << std::setprecision(2)
                  << "[PRICE] $" << price << "\n";
        logger.logPrice(price);
        algo.onPrice(price);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    algo_trader.printPortfolio(engine.getPrice());
    algo.printStats();

    return 0;
}