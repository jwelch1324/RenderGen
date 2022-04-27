cmake_minimum_required(VERSION 3.0.0)
project(tinyrenderer VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_FLAGS "-g -O0 -ggdb")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory("engine")

link_directories("/usr/local/lib")
add_executable(tinyrenderer src/main.cpp src/tgaimage.cpp src/model.cpp src/geometry.cpp)
target_include_directories(tinyrenderer PRIVATE include)
#target_link_libraries(tinyrenderer profiler)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
