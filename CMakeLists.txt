cmake_minimum_required(VERSION 3.15)
project(counting_network)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS thread system)

add_subdirectory(src)
add_subdirectory(test)
