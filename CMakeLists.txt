cmake_minimum_required(VERSION 3.0.0)
project(RenGen VERSION 0.1.0)


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

enable_testing()
add_subdirectory("engine")

find_package(SDL2 REQUIRED)

link_directories("/usr/local/lib")
file(GLOB_RECURSE MAIN_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(RenGen ${MAIN_SRC_FILES})
target_include_directories(RenGen PRIVATE include engine/include)
include_directories(RenGen ${SDL2_INCLUDE_DIRS})
message(${SDL2_INCLUDE_DIRS})
target_link_libraries(RenGen rendergen)
target_link_libraries(RenGen ${SDL2_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
