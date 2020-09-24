cmake_minimum_required(VERSION 3.16)
project(ImageLib)

set(CMAKE_CXX_STANDARD 14)

add_executable(ImageLib main.cpp Matrix.h Image.cpp Image.h util.cpp util.h core.cpp core.h main.cpp)