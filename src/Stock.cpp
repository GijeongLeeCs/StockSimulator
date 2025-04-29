#include "Stock.h"
#include "AlphaVantage.h"

Stock::Stock(const std::string& symbol, double price)
    : symbol(symbol), basePrice(price), currentPrice(price) {
    history = AlphaVantage::getDailyPrices(symbol);
    if (history.empty()) history.push_back(price);
}

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

void Stock::updatePriceFromAPI() {
    double newPrice = AlphaVantage::getRealTimePrice(symbol);
    if (newPrice > 0) {
        setPrice(newPrice);
    }
}

void Stock::setPrice(double price) {
    std::lock_guard<std::mutex> lock(priceMutex);
    currentPrice = price;
    history.push_back(price);
}

double Stock::getPrice() const {
    std::lock_guard<std::mutex> lock(priceMutex);
    return currentPrice;
}

const std::string& Stock::getSymbol() const {
    return symbol;
}

const std::vector<double>& Stock::getHistory() const {
    std::lock_guard<std::mutex> lock(priceMutex);
    return history;
}