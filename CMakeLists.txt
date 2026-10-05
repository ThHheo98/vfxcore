cmake_minimum_required(VERSION 3.25)

project(vfxcore LANGUAGES CXX)

add_executable(vfxcore src/main.cpp)
target_compile_features(vfxcore PRIVATE cxx_std_20)