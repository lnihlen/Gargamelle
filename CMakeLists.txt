cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.9)

project(Gargamelle VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_subdirectory(third_party)
add_subdirectory(src)

