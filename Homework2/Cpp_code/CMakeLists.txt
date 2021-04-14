cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.2 REQUIRED)

find_package(Python2 COMPONENTS Interpreter Development)
if (${Python2_FOUND})
    message("found python2")
    include_directories(${Python2_INCLUDE_DIRS})
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(imageUtils lib/imageUtils.cpp)

add_executable(grayLevelTransformation src/grayLevelTransformation.cpp include/matplotlibcpp.h)
target_link_libraries(grayLevelTransformation ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})

add_executable(histogramEqualization src/histogramEqualization.cpp include/matplotlibcpp.h)
target_link_libraries(histogramEqualization ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})

add_executable(histEquOpenCV src/OpenCVHistogramEqualization.cpp)
target_link_libraries(histEquOpenCV ${OpenCV_LIBS} imageUtils)

