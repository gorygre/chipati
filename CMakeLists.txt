cmake_minimum_required(VERSION 4.2.3)
project(chipati CXX)

set(CMAKE_CXX_COMPILER clang)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
  SDL3::SDL3-shared
  SDL3_image::SDL3_image-shared)
