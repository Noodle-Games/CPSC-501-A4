cmake_minimum_required(VERSION 3.26)
project(cpsc_501_a4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cpsc_501_a4
        Convolve.cpp
        Convolve.h
        WavReader.h
        WavReader.cpp
        TimeDomain.h
        TimeDomain.cpp
        FastFourier.h
        FastFourier.cpp)
