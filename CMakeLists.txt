cmake_minimum_required(VERSION 3.10)
project(StockSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StockSimulator
    src/main.cpp
    src/Stock.cpp
    src/Utils.cpp
    src/Market.cpp
)

target_include_directories(StockSimulator PRIVATE src)

find_package(Threads REQUIRED)
target_link_libraries(StockSimulator PRIVATE Threads::Threads)