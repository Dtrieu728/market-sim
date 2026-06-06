cmake_minimum_required(VERSION 3.15)
project(MarketSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_executable(MarketSim
    src/main.cpp
    src/OrderBook.cpp
    src/Trader.cpp
    src/Algorithm.cpp
    src/SMAAlgorithm.cpp
    src/Logger.cpp
    src/PriceQueue.cpp
    src/AlpacaFeed.cpp
    include/configLoader.hpp
)

target_include_directories(MarketSim PRIVATE include)

target_compile_definitions(MarketSim PRIVATE BOOST_ERROR_CODE_HEADER_ONLY)

target_link_libraries(MarketSim
    PRIVATE
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
)

enable_testing()

add_executable(TestOrderBook
    tests/test_orderbook.cpp
    src/OrderBook.cpp
    src/Logger.cpp
)

target_include_directories(TestOrderBook PRIVATE include)
target_link_libraries(TestOrderBook PRIVATE Threads::Threads)

add_test(NAME OrderBookTest COMMAND TestOrderBook)