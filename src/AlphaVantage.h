#pragma once
#include <string>

class AlphaVantage {
public:
    static double getRealTimePrice(const std::string& symbol);
    static std::vector<double> getDailyPrices(const std::string& symbol);
};