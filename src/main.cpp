#include "AlpacaFeed.hpp"
#include "OrderBook.hpp"
#include "Trader.hpp"
#include "Algorithm.hpp"
#include "Logger.hpp"
#include "PriceQueue.hpp"
#include "SMAAlgorithm.hpp"
#include "configLoader.hpp"
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

    const char* key    = std::getenv("ALPACA_API_KEY");
    const char* secret = std::getenv("ALPACA_API_SECRET");

    // cout << "KEY: "    << (key    ? key    : "NULL") << "\n";
    // cout << "SECRET: " << (secret ? secret : "NULL") << "\n";

     if (!key || !secret) {
        std::cerr << "[MAIN] Set ALPACA_API_KEY and ALPACA_API_SECRET\n";
        return 1;
    }

    SimConfig cfg = loadConfig("../strategy_config.json");
    Logger logger("prices.csv", "trades.csv");
    PriceQueue queue;
    AlpacaFeed feed(key, secret, "AAPL", queue, logger);
    // PriceEngine engine(100.0, 0.5, queue, logger);
    // if(argc > 1){
    //     engine.loadCSV(argv[1]);
    // }

    OrderBook book(logger);
    Trader algo_trader("AlgoTrader", 10000.0);
    SMAAlgorithm algo(algo_trader, book,queue,5, 0.002);

    // launch threads
    std::thread feedThread([&]()  { feed.run();  });
    std::thread algoThread([&]()  { algo.run();  });

    std::thread watchThread([&]() {
        while (g_running) std::this_thread::sleep_for(std::chrono::milliseconds(200));
        feed.stop();
    });

    // main thread waits for Ctrl+C
    feedThread.join();
    algoThread.join();
    watchThread.join();

    algo_trader.printPortfolio(feed.getPrice());
    

    return 0;
}