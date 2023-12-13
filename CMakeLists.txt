cmake_minimum_required(VERSION 3.27)
project(
    Assignment3
    LANGUAGES CXX
    VERSION 1.0
)

set(CMAKE_CXX_STANDARD 20)

find_package(ftxui)

add_executable(main src/WordGraph.cpp include/WordGraph.hpp
        src/main.cpp)

target_include_directories(main PRIVATE src)

target_link_libraries(main
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)
