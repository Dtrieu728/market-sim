cmake_minimum_required(VERSION 3.15)
project(MarketSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(MarketSim
    src/main.cpp
    src/OrderBook.cpp
    src/Trader.cpp
    src/Algorithm.cpp
    src/SMAAlgorithm.cpp
    src/Logger.cpp
    src/PriceQueue.cpp
    src/AlpacaFeed.cpp
    tests/test_orderbook.cpp
    include/configLoader.hpp
)

target_link_libraries(MarketSim
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::system
)

enable_testing()
add_executable(TestOrderBook
    tests/test_orderbook.cpp
    src/OrderBook.cpp
    src/Logger.cpp
)
target_include_directories(TestOrderBook PRIVATE include)
target_link_libraries(TestOrderBook Threads::Threads)
add_test(NAME OrderBookTest COMMAND TestOrderBook)