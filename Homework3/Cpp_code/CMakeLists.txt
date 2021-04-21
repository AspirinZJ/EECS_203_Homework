cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.4 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(imageUtils lib/imageUtils.cpp)

add_executable(linearAverageFilter src/linearAverageFilter.cpp)
target_link_libraries(linearAverageFilter PRIVATE
        ${OpenCV_LIBS} imageUtils)

add_executable(opencvFilters src/opencvConvAndFilters.cpp)
target_link_libraries(opencvFilters PRIVATE ${OpenCV_LIBS})

add_executable(medianFilter src/medianFilter.cpp)
target_link_libraries(medianFilter PRIVATE
        ${OpenCV_LIBS} imageUtils)