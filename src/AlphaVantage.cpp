#include "AlphaVantage.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

double AlphaVantage::getRealTimePrice(const std::string& symbol) {
    CURL* curl = curl_easy_init();
    std::string response;
    const std::string apiKey = "YOUR_API_KEY"; // Replace with your key
    const std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + 
                          symbol + "&apikey=" + apiKey;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        try {
            json j = json::parse(response);
            return std::stod(j["Global Quote"]["05. price"].get<std::string>());
        } catch (...) {
            return 0.0;
        }
    }
    return 0.0;
}

std::vector<double> AlphaVantage::getDailyPrices(const std::string& symbol) {
    std::vector<double> prices;
    CURL* curl = curl_easy_init();
    std::string response;
    const std::string apiKey = "YOUR_API_KEY"; // Replace with your key
    const std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + 
                          symbol + "&apikey=" + apiKey + "&outputsize=compact";

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        try {
            json j = json::parse(response);
            for (auto& [date, values] : j["Time Series (Daily)"].items()) {
                prices.push_back(std::stod(values["4. close"].get<std::string>()));
                if (prices.size() >= 30) break; // Get last 30 days
            }
        } catch (...) {}
    }
    return prices;
}