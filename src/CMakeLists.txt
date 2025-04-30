cmake_minimum_required(VERSION 3.5)

project(HeaderDetCPPInference VERSION 0.1)

set(LIBRARY_NAME "header_detector")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
message("sub CMakeLists.txt")

# CUDA
set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
find_package(CUDA 11 REQUIRED)

set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
# !CUDA

# OpenCV
# find_package(OpenCV REQUIRED)
# set(OpenCV_INCLUDE_DIRS "/home/gzjh/gzjh/docker/opencv-4.11.0/include/opencv2")
include_directories(
    # ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/opencv2
    )
# !OpenCV
file(GLOB_RECURSE HEADER 
    ${CMAKE_SOURCE_DIR}/include/*.h*
    )
file(GLOB_RECURSE SOURCE 
    ${CMAKE_SOURCE_DIR}/src/*.c*
    )

add_library(
    ${LIBRARY_NAME}
    # STATIC  # 静态库
    SHARED #动态库
    ${HEADER}  # 自己调用的头文件全部打包在静态库中，提供给外部使用的头文件放在model_params.hpp和model_func.cpp中
    ${SOURCE}
)

set(OpenCV_LIBS
    ${CMAKE_SOURCE_DIR}/libs/libopencv_core.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_imgproc.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_highgui.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_dnn.so
    # ${CMAKE_SOURCE_DIR}/libs/libopencv_imgcodecs.so
)
target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS})