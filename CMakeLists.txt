cmake_minimum_required(VERSION 3.5)

set (ProjectName Yolov8CPPInference)
project(${ProjectName} VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# CUDA
set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
find_package(CUDA 11 REQUIRED)

set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
# !CUDA


include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/opencv2)

# 添加算法库目标
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set(OpenCV_LIBS
    ${CMAKE_SOURCE_DIR}/libs/libopencv_core.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_imgproc.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_highgui.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_imgcodecs.so
    ${CMAKE_SOURCE_DIR}/libs/libopencv_dnn.so
)
add_executable(${ProjectName} main.cpp)

set_target_properties(${ProjectName} PROPERTIES LINK_FLAGS "-Wl,--no-as-needed")
target_link_libraries(${ProjectName} PRIVATE
    header_detector  # 算法库名称
    ${OpenCV_LIBS}
    dl
    )
target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/opencv2
)