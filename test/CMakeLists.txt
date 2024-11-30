cmake_minimum_required(VERSION 3.21)
project(BignumTest VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(bignum_test test.cpp)
target_link_libraries(bignum_test PUBLIC boost_unit_test_framework)

