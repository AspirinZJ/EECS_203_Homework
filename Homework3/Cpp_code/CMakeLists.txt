cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.4 REQUIRED)

find_package(Python2 COMPONENTS Interpreter Development)
if (${Python2_FOUND})
    message("found python2")
    include_directories(${Python2_INCLUDE_DIRS})
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(imageUtils lib/imageUtils.cpp)

add_executable(linearAverageFilter src/linearAverageFilter.cpp include/matplotlibcpp.h)
target_link_libraries(linearAverageFilter PRIVATE
        ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})

add_executable(opencvFilters src/opencvConvAndFilters.cpp)
target_link_libraries(opencvFilters PRIVATE ${OpenCV_LIBS})

add_executable(medianFilter src/medianFilter.cpp include/matplotlibcpp.h)
target_link_libraries(medianFilter PRIVATE
        ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})

add_executable(opencvMedianFilter src/opencvMedianFilter.cpp)
target_link_libraries(opencvMedianFilter PRIVATE ${OpenCV_LIBS})

add_executable(homeQ3 src/homeworkQuestion3.cpp include/matplotlibcpp.h)
target_link_libraries(homeQ3 PRIVATE ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})