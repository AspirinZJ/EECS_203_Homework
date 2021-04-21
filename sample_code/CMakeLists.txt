# 必须， 指明cmake版本
cmake_minimum_required(VERSION 3.19)

# 必须写， 项目名称
project(sample_code)

# 非必须， 指明C++标准
set(CMAKE_CXX_STANDARD 14)

# find opencv
find_package(OpenCV 3.2 REQUIRED)

find_package(Python2 COMPONENTS Interpreter Development)
if (${Python2_FOUND})
    include_directories(${Python2_INCLUDE_DIRS})
endif ()

# 添加一个库
add_library(imageUtils src/imageUtils.cpp)

# 添加一个可执行文件
add_executable(main src/main.cpp src/matplotlibcpp.h)
target_link_libraries(main ${OpenCV_LIBS} imageUtils ${Python2_LIBRARIES})

add_executable(namespaceDemo src/namespaceDemo.cpp)

add_executable(linearAvgFilter src/linearAvgFilter.cpp)
target_link_libraries(linearAvgFilter ${OpenCV_LIBS})

add_executable(structDemo src/structDemo.cpp)

add_executable(memoryDemo src/memoryDemo.cpp)

add_executable(ref src/referenceDemo.cpp)