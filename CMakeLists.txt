cmake_minimum_required(VERSION 3.10)
project(StockSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.9.1 REQUIRED)
find_package(Threads REQUIRED)

add_executable(StockSimulator
    src/main.cpp
    src/Stock.cpp
    src/Market.cpp
    src/Utils.cpp
    src/YahooFinance.cpp
)

target_include_directories(StockSimulator PRIVATE src)
target_link_libraries(StockSimulator PRIVATE
    Threads::Threads
    CURL::libcurl
    nlohmann_json::nlohmann_json
)