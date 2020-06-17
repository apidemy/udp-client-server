cmake_minimum_required(VERSION 3.15)
project(udpclient)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB SRC *.cpp)

find_package(Boost REQUIRED)

SET(LIBS ${Boost_SYSTEM_LIBRARY} -lpthread)

add_executable(udpclient ${SRC})
target_link_libraries(udpclient ${LIBS})