#include "Stock.h"
#include <algorithm>

thread_local std::mt19937 Stock::gen(std::random_device{}());

Stock::Stock(const std::string& symbol, double price)
    : symbol(symbol), basePrice(price), currentPrice(price) {}

Stock::Stock(Stock&& other) noexcept
    : symbol(std::move(other.symbol)),
      basePrice(other.basePrice),
      currentPrice(other.currentPrice),
      history(std::move(other.history)) {}

Stock& Stock::operator=(Stock&& other) noexcept {
    if (this != &other) {
        symbol = std::move(other.symbol);
        basePrice = other.basePrice;
        currentPrice = other.currentPrice;
        history = std::move(other.history);
    }
    return *this;
}

void Stock::updatePrice() {
    std::lock_guard<std::mutex> lock(priceMutex);
    std::normal_distribution<double> dist(0.0, 2.0);
    currentPrice += dist(gen) * basePrice * 0.01;
    history.push_back(currentPrice);
}

double Stock::getPrice() const {
    return currentPrice;
}

const std::string& Stock::getSymbol() const {
    return symbol;
}

const std::vector<double>& Stock::getHistory() const {
    return history;
}