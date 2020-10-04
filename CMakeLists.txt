cmake_minimum_required(VERSION 3.16)
project(ImageLib)

set(CMAKE_CXX_STANDARD 14)

file(GLOB ImageLib_SRC
        "include/*.hpp"
        "src/*.cpp"
        )

add_executable(ImageLib main.cpp ${ImageLib_SRC})
target_include_directories(ImageLib PRIVATE "include")

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()