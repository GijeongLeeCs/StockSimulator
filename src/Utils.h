#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <mutex>
#include <fstream>

class Stock;

namespace StockUtils {
    inline std::string formatPrice(double price) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << "$" << price;
        return oss.str();
    }

    template <typename T>
    T calculateAverage(const std::vector<T>& data) {
        if (data.empty()) return T();
        return std::accumulate(data.begin(), data.end(), T()) / data.size();
    }

    class Logger {
    public:
        static void log(const std::string& message) {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout << "[LOG] " << message << "\n";
        }
    private:
        static std::mutex logMutex;
    };

    void exportToCSV(const std::vector<Stock>& stocks, const std::string& filename);

    template <typename Container, typename Predicate>
    Container filter(const Container& data, Predicate pred) {
        Container result;
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), pred);
        return result;
    }
}