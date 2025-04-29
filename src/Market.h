#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "Stock.h"

class Market {
public:
    Market();
    ~Market();

    void start();
    void stop();
    void printPrices() const;
    const std::vector<Stock>& getStocks() const;

private:
    void runSimulation();

    std::vector<Stock> stocks;
    std::thread marketThread;
    std::atomic<bool> isRunning;
    mutable std::mutex ioMutex;
};