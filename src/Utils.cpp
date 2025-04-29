#include "Utils.h"
#include "Stock.h"

std::mutex StockUtils::Logger::logMutex;

void StockUtils::exportToCSV(const std::vector<Stock>& stocks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::log("Failed to open file: " + filename);
        return;
    }

    file << "Symbol,CurrentPrice,AveragePrice\n";
    for (const auto& stock : stocks) {
        const auto& history = stock.getHistory();
        file << stock.getSymbol() << ","
             << formatPrice(stock.getPrice()) << ","
             << formatPrice(calculateAverage(history)) << "\n";
    }
    Logger::log("Exported data to " + filename);
}