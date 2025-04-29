#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <iostream>  // Add this for std::cerr

class YahooFinance {
private:
    static std::chrono::system_clock::time_point lastRequestTime;
    static const std::chrono::milliseconds REQUEST_DELAY;  // Changed from constexpr
public:
    static double getRealTimePrice(const std::string& symbol);
    static std::vector<double> getDailyPrices(const std::string& symbol);
};