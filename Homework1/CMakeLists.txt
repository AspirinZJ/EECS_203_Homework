cmake_minimum_required(VERSION 3.17)
project(Homework1)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.2 REQUIRED)

add_executable(subsample_and_interpolate src/subsample_and_interpolate.c)