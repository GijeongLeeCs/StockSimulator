#include "Utils.h"
#include "Stock.h"
#include <numeric>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

std::mutex logMutex;  // Moved here from Utils.h

std::string StockUtils::formatPrice(double price) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "$" << price;
    return oss.str();
}

double StockUtils::calculateAverage(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

void StockUtils::exportToCSV(const std::vector<Stock>& stocks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cerr << "[ERROR] Failed to open " << filename << "\n";
        return;
    }

    file << "Symbol,CurrentPrice,30DayAvg,High,Low\n";
    for (const auto& stock : stocks) {
        const auto& history = stock.getHistory();
        auto last30 = history.size() > 30 ? 
            std::vector<double>(history.end()-30, history.end()) : history;
        
        file << stock.getSymbol() << ","
             << StockUtils::formatPrice(stock.getPrice()) << ","
             << StockUtils::formatPrice(calculateAverage(last30)) << ","
             << StockUtils::formatPrice(*max_element(last30.begin(), last30.end())) << ","
             << StockUtils::formatPrice(*min_element(last30.begin(), last30.end())) << "\n";
    }

    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[LOG] Exported to " << filename << "\n";
}