cmake_minimum_required(VERSION 3.10)
project(DoublePendulum)

set(CMAKE_CXX_STANDARD 14)


add_library(utils STATIC utils.cpp utils.h)

add_subdirectory(imgui-sfml)
add_executable(DoublePendulum main.cpp)

target_link_libraries(DoublePendulum utils sfml-graphics sfml-system sfml-window ImGui-SFML)
target_link_libraries(DemoWindow sfml-graphics sfml-system sfml-window ImGui-SFML)
