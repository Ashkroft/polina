cmake_minimum_required(VERSION 3.5)
project(rational)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES rational.cpp)
add_library(rational ${SOURCE_FILES})

install(TARGETS rational DESTINATION lib)
install(FILES rational.h DESTINATION include)
