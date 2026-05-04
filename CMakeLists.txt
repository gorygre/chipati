cmake_minimum_required(VERSION 4.1.2)
project(chipati CXX)

set(CMAKE_CXX_COMPILER clang++)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3-shared)
