cmake_minimum_required(VERSION 3.0.0)
project(rendergen VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-g -O0 -ggdb")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

link_directories("/usr/local/lib")
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/tests *.cpp)
add_library(rendergen STATIC ${SRC_FILES})
target_include_directories(rendergen PRIVATE include)
#target_link_libraries(tinyrenderer profiler)

#add_subdirectory("thirdparty/benchmark")
set(benchmark_DIR "../thirdparty/benchmark/build")
find_package(benchmark REQUIRED)

enable_testing()
file(GLOB_RECURSE TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cc)
add_executable(rendergentest ${TEST_SRC_FILES})
target_link_libraries(rendergentest gtest_main)
target_link_libraries(rendergentest rendergen)
target_include_directories(rendergentest PRIVATE ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE BENCH_SRC_FILES ${PROJECT_SOURCE_DIR}/benchmarks/*.cc)
add_executable(rendergenbench ${BENCH_SRC_FILES})
target_link_libraries(rendergenbench rendergen benchmark::benchmark)
target_include_directories(rendergenbench PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(rendergentest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
