cmake_minimum_required(VERSION 3.19)
project(Cpp_code)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

set(EXE_LIST "") # create a list
add_executable(laplacian src/laplacian_filter.cpp)
list(APPEND EXE_LIST laplacian)
add_executable(filter2d_built_in src/laplacian_OpenCV_built_in.cpp)
list(APPEND EXE_LIST filter2d_built_in)
add_executable(DFT src/DFT.cpp)
list(APPEND EXE_LIST DFT)

add_executable(DFT_OpenCV src/DFT_OpenCV.cpp)
list(APPEND EXE_LIST DFT_OpenCV)

# add_executable(test src/test.cpp)
# list(APPEND EXE_LIST test)

foreach (EXE IN LISTS EXE_LIST)
    target_link_libraries(${EXE} ${OpenCV_LIBS})
endforeach ()