cmake_minimum_required(VERSION 3.0.0)
project(tinyrenderer VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-g -O0 -ggdb")

include(CTest)
enable_testing()

link_directories("/usr/local/lib")
add_executable(tinyrenderer src/main.cpp src/tgaimage.cpp src/model.cpp)
target_include_directories(tinyrenderer PRIVATE include)
#target_link_libraries(tinyrenderer profiler)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
