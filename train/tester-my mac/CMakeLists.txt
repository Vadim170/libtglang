cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(tglang VERSION 1.0.0 LANGUAGES C CXX)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ldl")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(tglang-tester tglang-tester.cpp)

# find_library(tglang_LIBRARY tglang PATHS "${CMAKE_CURRENT_SOURCE_DIR}" NO_DEFAULT_PATH)
# target_link_libraries(tglang-tester PRIVATE "${tglang_LIBRARY}" dl)
# target_link_libraries(tglang-tester PRIVATE "${tglang_LIBRARY}")

# set_target_properties(tglang-tester PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
# set_target_properties(tglang-tester PROPERTIES LINK_FLAGS "-Wl,-rpath,./submission/")
# set_target_properties(tglang-tester PROPERTIES
#     LINK_FLAGS "-Wl,-rpath,./;-Wl,-rpath,./submission/")

if (NOT WIN32)
#   target_link_libraries(tglang-tester PRIVATE m)
  target_link_libraries(tglang-tester PRIVATE m dl)
  if (UNIX AND NOT APPLE)
    target_link_libraries(tglang-tester PRIVATE rt)
  endif()
else()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()