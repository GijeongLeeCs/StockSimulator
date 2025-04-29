#include "YahooFinance.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <sstream>

using json = nlohmann::json;

// Initialize static members
std::chrono::system_clock::time_point YahooFinance::lastRequestTime = 
    std::chrono::system_clock::now();
const std::chrono::milliseconds YahooFinance::REQUEST_DELAY{2000};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

double YahooFinance::getRealTimePrice(const std::string& symbol) {
    // Enforce rate limiting
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - lastRequestTime;
    if (elapsed < REQUEST_DELAY) {
        std::this_thread::sleep_for(REQUEST_DELAY - elapsed);
    }
    lastRequestTime = std::chrono::system_clock::now();

    CURL* curl = curl_easy_init();
    std::string response;
    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + symbol + "?interval=1d&range=1d";

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
        headers = curl_slist_append(headers, "Accept: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "CURL failed: " << curl_easy_strerror(res) << "\n";
            return 0.0;
        }

        try {
            json j = json::parse(response);
            return j["chart"]["result"][0]["meta"]["regularMarketPrice"].get<double>();
        } catch (const std::exception& e) {
            std::cerr << "Error parsing " << symbol << ": " << e.what() << "\n";
            return 0.0;
        }
    }
    return 0.0;
}

std::vector<double> YahooFinance::getDailyPrices(const std::string& symbol) {
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - lastRequestTime;
    if (elapsed < REQUEST_DELAY) {
        std::this_thread::sleep_for(REQUEST_DELAY - elapsed);
    }
    lastRequestTime = std::chrono::system_clock::now();

    CURL* curl = curl_easy_init();
    std::string response;
    std::time_t now_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t past_t = now_t - (30 * 24 * 3600);
    
    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + symbol + 
                     "?interval=1d&period1=" + std::to_string(past_t) + 
                     "&period2=" + std::to_string(now_t);

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return {};
        }

        try {
            json j = json::parse(response);
            std::vector<double> prices;
            for (auto& quote : j["chart"]["result"][0]["indicators"]["quote"][0]["close"]) {
                if (!quote.is_null()) {
                    prices.push_back(quote.get<double>());
                }
            }
            return prices;
        } catch (...) {
            return {};
        }
    }
    return {};
}