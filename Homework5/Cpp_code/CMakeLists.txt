cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

find_package(OpenCV 3.3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(filtering src/filtering.cpp)
target_link_libraries(filtering ${OpenCV_LIBS})

