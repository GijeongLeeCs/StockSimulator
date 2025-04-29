#include "Market.h"
#include <iostream>
#include <iomanip>

int main() {
    Market market;
    market.start();

    std::cout << "=== Real-Time Stock Simulator ===\n";
    std::cout << "Press Enter to stop...\n";

    while (true) {
        market.printPrices();
        std::cout << "-----------------\n";
        
        if (std::cin.rdbuf()->in_avail() > 0) break;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    market.stop();
    return 0;
}