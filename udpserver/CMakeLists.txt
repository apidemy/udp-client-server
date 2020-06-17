cmake_minimum_required(VERSION 3.12)
project(udpserver)

set(CMAKE_CXX_STANDARD 14)

SET(LIBS pthread)

include_directories(${PROJECT_SOURCE_DIR})

FILE(GLOB SRC *.cpp)

find_package(Boost 1.65.0 REQUIRED)

add_executable(udpserver main.cpp ${SRC})

target_link_libraries(udpserver ${LIBS})