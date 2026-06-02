cmake_minimum_required(VERSION 3.15)
project(MarketSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(MarketSim
    src/main.cpp
    src/PriceEngine.cpp
)