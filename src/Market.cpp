#include "Market.h"
#include <iostream>
#include <iomanip>
#include <chrono>

Market::Market() : isRunning(false) {
    stocks.emplace_back("AAPL", 150.0);
    stocks.emplace_back("GOOG", 2800.0);
    stocks.emplace_back("TSLA", 700.0);
}

Market::~Market() { stop(); }

void Market::start() {
    isRunning = true;
    marketThread = std::thread(&Market::runSimulation, this);
}

void Market::stop() {
    isRunning = false;
    if (marketThread.joinable()) marketThread.join();
}

void Market::printPrices() const {
    std::lock_guard<std::mutex> lock(ioMutex);
    for (const auto& stock : stocks) {
        std::cout << stock.getSymbol() << ": $" 
                  << std::fixed << std::setprecision(2) 
                  << stock.getPrice() << "\n";
    }
}

const std::vector<Stock>& Market::getStocks() const {
    return stocks;
}

void Market::runSimulation() {
    while (isRunning) {
        for (auto& stock : stocks) {
            stock.updatePriceFromAPI();
        }
        std::this_thread::sleep_for(std::chrono::minutes(1)); // Respect API rate limits
    }
}