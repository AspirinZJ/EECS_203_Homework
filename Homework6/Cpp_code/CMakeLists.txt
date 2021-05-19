cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.3 REQUIRED)

add_executable(Gaussian_and_inverse src/homework6.cpp)
target_link_libraries(Gaussian_and_inverse ${OpenCV_LIBS})

add_executable(test src/test.cpp)
target_link_libraries(test ${OpenCV_LIBS})
