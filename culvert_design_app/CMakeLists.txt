cmake_minimum_required(VERSION 3.5)

project(culvert_design_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(culvert_design_app main.cpp culverts.h)
