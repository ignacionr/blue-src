cmake_minimum_required(VERSION 3.19)
project(blue-src)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable target
add_executable(blue-src src/main.cpp)

if (MSVC)
    # target_precompile_headers(beatograph PRIVATE "src/pch.h")
    target_compile_options(blue-src PRIVATE /W4 /WX /MP /Zi /Od /bigobj)
    target_link_options(blue-src PRIVATE /INCREMENTAL)
endif()

find_package(CURL CONFIG REQUIRED)

target_link_libraries(blue-src PRIVATE CURL::libcurl)