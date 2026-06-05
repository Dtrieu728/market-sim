#pragma once
#include <string>
#include <fstream>
#include <iostream>

struct SimConfig {
    std::string strategy;
    int shortPeriod  = 5;
    int longPeriod   = 20;
    double capital   = 10000.0;
    bool loaded      = false;
};

inline SimConfig loadConfig(const std::string& path) {
    SimConfig cfg;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "[CONFIG] No config file found, using defaults\n";
        return cfg;
    }

    // simple key:value JSON parsing without a library
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("short_period") != std::string::npos)
            cfg.shortPeriod = std::stoi(line.substr(line.find(':') + 1));
        else if (line.find("long_period") != std::string::npos)
            cfg.longPeriod = std::stoi(line.substr(line.find(':') + 1));
        else if (line.find("initial_capital") != std::string::npos)
            cfg.capital = std::stod(line.substr(line.find(':') + 1));
        else if (line.find("strategy") != std::string::npos) {
            auto start = line.find('"', line.find(':')) + 1;
            auto end   = line.find('"', start);
            cfg.strategy = line.substr(start, end - start);
        }
    }

    cfg.loaded = true;
    std::cout << "[CONFIG] Loaded: " << cfg.strategy
              << " short=" << cfg.shortPeriod
              << " long=" << cfg.longPeriod << "\n";
    return cfg;
}