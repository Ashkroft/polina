cmake_minimum_required (VERSION 3.3)
project (Rational)

add_library (rational rational.cpp)

install (TARGETS rational DESTINATION lib)
install (FILES rational.h DESTINATION include)
