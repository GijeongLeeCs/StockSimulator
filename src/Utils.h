#pragma once
#include <vector>
#include <string>
#include <mutex>

class Stock;

namespace StockUtils {
    // Declaration only (implementation in .cpp)
    std::string formatPrice(double price);
    double calculateAverage(const std::vector<double>& data);
    void exportToCSV(const std::vector<Stock>& stocks, const std::string& filename);
}