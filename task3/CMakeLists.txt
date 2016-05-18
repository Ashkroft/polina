cmake_minimum_required(VERSION 3.5)
project(rational)

add_library(rational SHARED rational.cpp)

install (FILES rational.h DESTINATION include)
install (TARGETS rational DESTINATION lib)
