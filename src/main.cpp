#include "PriceEngine.hpp"
#include "OrderBook.hpp"
#include "Trader.hpp"
#include "Algorithm.hpp"
#include "Logger.hpp"
#include "PriceQueue.hpp"
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>
using namespace std;

atomic<bool> g_running(true);

void signalHandler(int) {
    cout << "\n[MAIN] Shutting down...\n";
    g_running = false;
}

int main(int argc , char* argv[]) {
    signal(SIGINT, signalHandler);

    Logger logger("prices.csv", "trades.csv");
    PriceQueue queue;

    PriceEngine engine(100.0, 0.5, queue, logger);
    if(argc > 1){
        engine.loadCSV(argv[1]);
    }

    OrderBook book(logger);
    Trader algo_trader("AlgoTrader", 10000.0);
    Algorithm algo(algo_trader, book,5, 0.002,queue);

    // launch threads
    thread priceThread([&]() {
        engine.run();
    });

    thread algoThread([&]() {
        algo.run();
    });

    // main thread waits for Ctrl+C
    priceThread.join();
    algoThread.join();

    algo_trader.printPortfolio(engine.getPrice());
    algo.printStats();

    return 0;
}