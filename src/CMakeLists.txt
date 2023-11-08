cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(tglang VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

set(VOCAB_FILE /resources/vocab.json)
set(MERGES_FILE /resources/merges.txt)
set(ONNX_FILE /resources/model.onnx)
configure_file(
	"${PROJECT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_BINARY_DIR}/src/config.hpp"
)

option(ONNXRUNTIME_ROOTDIR "onnxruntime root dir")

if(NOT ONNXRUNTIME_ROOTDIR)
	set(ONNXRUNTIME_ROOTDIR "onnxruntime-linux-x64-1.15.1")
endif()

include_directories(
	"${ONNXRUNTIME_ROOTDIR}/include"                           # Pre-built package
	"${ONNXRUNTIME_ROOTDIR}/include/onnxruntime"               # Linux local install to /usr/local
	"${PROJECT_BINARY_DIR}/src"
)

link_directories("${ONNXRUNTIME_ROOTDIR}/lib")

add_library(tglang SHARED	tglang.cpp
							simdjson.cpp)

target_link_libraries(tglang PRIVATE onnxruntime)

set_target_properties(tglang PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN/resources/")
