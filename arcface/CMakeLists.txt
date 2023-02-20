cmake_minimum_required(VERSION 2.6)

project(arcface)

add_definitions(-std=c++11)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)

# ----------- for fixed path ------------------ # 
set(TENSORRT_ROOT /usr/local/TensorRT-8.0.1.6)
find_path(TENSORRT_INCLUDE_DIR NvInfer.h
 HINTS ${TENSORRT_ROOT} ${CUDA_TOOLKIT_ROOT_DIR}
 PATH_SUFFIXES include)
MESSAGE(STATUS "Found TensorRT headers at ${TENSORRT_INCLUDE_DIR}")

find_library(TENSORRT_LIBRARY_INFER nvinfer
 HINTS ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
 PATH_SUFFIXES lib lib64 lib/x64)

find_library(TENSORRT_LIBRARY_INFER_PLUGIN nvinfer_plugin
 HINTS ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
 PATH_SUFFIXES lib lib64 lib/x64)

set(TENSORRT_LIBRARY ${TENSORRT_LIBRARY_INFER} ${TENSORRT_LIBRARY_INFER_PLUGIN})
 MESSAGE(STATUS "Find TensorRT libs at ${TENSORRT_LIBRARY}")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    message("embed_platform on")
    include_directories(/usr/local/cuda/targets/aarch64-linux/include)
    link_directories(/usr/local/cuda/targets/aarch64-linux/lib)
else()
    message("embed_platform off")
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

cuda_add_library(myplugins SHARED ${PROJECT_SOURCE_DIR}/prelu.cu)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(arcface-r50 ${PROJECT_SOURCE_DIR}/arcface-r50.cpp)
target_link_libraries(arcface-r50 ${TENSORRT_LIBRARY})
target_link_libraries(arcface-r50 cudart)
target_link_libraries(arcface-r50 myplugins)
target_link_libraries(arcface-r50 ${OpenCV_LIBS})

add_executable(arcface-mobilefacenet ${PROJECT_SOURCE_DIR}/arcface-mobilefacenet.cpp)
target_link_libraries(arcface-mobilefacenet ${TENSORRT_LIBRARY})
target_link_libraries(arcface-mobilefacenet cudart)
target_link_libraries(arcface-mobilefacenet myplugins)
target_link_libraries(arcface-mobilefacenet ${OpenCV_LIBS})

add_executable(arcface-r100 ${PROJECT_SOURCE_DIR}/arcface-r100.cpp)
target_link_libraries(arcface-r100 ${TENSORRT_LIBRARY})
target_link_libraries(arcface-r100 cudart)
target_link_libraries(arcface-r100 myplugins)
target_link_libraries(arcface-r100 ${OpenCV_LIBS})

add_definitions(-O2 -pthread)

