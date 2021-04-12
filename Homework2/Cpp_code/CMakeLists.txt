cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.2 REQUIRED)

find_package(Python2 COMPONENTS Interpreter Development)
if (${Python2_FOUND})
    message("found python2")
    include_directories(${Python2_INCLUDE_DIRS})
endif ()

add_library(imageUtils src/imageUtils.cpp)

add_executable(main src/main.cpp src/matplotlibcpp.h)
target_link_libraries(main
        ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})