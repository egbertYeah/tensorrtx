cmake_minimum_required(VERSION 2.6)

project(retinaface)

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

cuda_add_library(decodeplugin SHARED ${PROJECT_SOURCE_DIR}/decode.cu)
target_link_libraries(decodeplugin ${TENSORRT_LIBRARY} cudart)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(retina_r50 ${PROJECT_SOURCE_DIR}/calibrator.cpp ${PROJECT_SOURCE_DIR}/retina_r50.cpp)
target_link_libraries(retina_r50 ${TENSORRT_LIBRARY})
target_link_libraries(retina_r50 cudart)
target_link_libraries(retina_r50 decodeplugin)
target_link_libraries(retina_r50 ${OpenCV_LIBRARIES})

add_executable(retina_mnet ${PROJECT_SOURCE_DIR}/calibrator.cpp ${PROJECT_SOURCE_DIR}/retina_mnet.cpp)
target_link_libraries(retina_mnet ${TENSORRT_LIBRARY})
target_link_libraries(retina_mnet cudart)
target_link_libraries(retina_mnet decodeplugin)
target_link_libraries(retina_mnet ${OpenCV_LIBRARIES})

add_executable(eval_r50 ${PROJECT_SOURCE_DIR}/calibrator.cpp ${PROJECT_SOURCE_DIR}/eval_r50.cpp)
target_link_libraries(eval_r50 ${TENSORRT_LIBRARY})
target_link_libraries(eval_r50 cudart)
target_link_libraries(eval_r50 decodeplugin)
target_link_libraries(eval_r50 ${OpenCV_LIBRARIES})

add_definitions(-O2 -pthread)

