cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(tglang)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wno-trigraphs")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(ONNXRUNTIME_ROOTDIR "onnxruntime-linux-x64-1.16.2")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
	set(ONNXRUNTIME_ROOTDIR "onnxruntime-osx-arm64-1.15.1")
else()
	message(FATAL_ERROR "Unsupported platform or architecture")
endif()

include_directories(
	"${ONNXRUNTIME_ROOTDIR}/include"                           # Pre-built package
)

link_directories("${ONNXRUNTIME_ROOTDIR}/lib")

add_library(tglang SHARED	tglang.cpp
							vocab_keys.cpp
							# merges_data.cpp
							# merges_data2.cpp
							model_wrapper.cpp)

target_link_libraries(tglang PRIVATE onnxruntime)
