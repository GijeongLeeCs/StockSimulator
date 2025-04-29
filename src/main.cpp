#include "Market.h"
#include "Utils.h"
#include <iostream>
#include <chrono>

int main() {
    Market market;
    market.start();

    std::cout << "=== Real-Time Stock Tracker ===\n";
    std::cout << "Press Enter to stop...\n\n";

    while (true) {
        market.printPrices();
        std::cout << "-----------------\n";
        
        StockUtils::exportToCSV(market.getStocks(), "stocks.csv");
        
        if (std::cin.rdbuf()->in_avail() > 0) break;
        std::this_thread::sleep_for(std::chrono::seconds(30)); // Update every 30 seconds
    }

    market.stop();
    return 0;
}