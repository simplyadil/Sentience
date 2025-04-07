cmake_minimum_required(VERSION 3.10)

# Compiler library
add_library(ml_compiler
    compiler.cpp
    ast_visitor.cpp
    code_generator.cpp
    runtime.cpp
)

target_include_directories(ml_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++17 standard
target_compile_features(ml_compiler PUBLIC cxx_std_17)

# Add compiler options
if(MSVC)
    target_compile_options(ml_compiler PRIVATE /W4)
else()
    target_compile_options(ml_compiler PRIVATE -Wall -Wextra -pedantic)
endif()
