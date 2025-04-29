#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <random>

class Stock {
public:
    Stock(const std::string& symbol, double price);
    
    // Delete copy operations
    Stock(const Stock&) = delete;
    Stock& operator=(const Stock&) = delete;
    
    // Move operations
    Stock(Stock&& other) noexcept;
    Stock& operator=(Stock&& other) noexcept;

    void updatePrice();
    double getPrice() const;
    const std::string& getSymbol() const;
    const std::vector<double>& getHistory() const;

private:
    std::string symbol;
    double basePrice;
    double currentPrice;
    std::vector<double> history;
    mutable std::mutex priceMutex;
    static thread_local std::mt19937 gen;
};