cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.3 REQUIRED)

add_executable(rgb src/rgb_image.cpp)

target_link_libraries(rgb ${OpenCV_LIBS})