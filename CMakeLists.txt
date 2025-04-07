cmake_minimum_required(VERSION 3.10)
project(ML_Language VERSION 1.0)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler subdirectory
add_subdirectory(compiler)

# Compiler executable
add_executable(ml_compiler_exe compiler_main.cpp)
target_link_libraries(ml_compiler_exe PRIVATE ml_compiler)

# Set output name
set_target_properties(ml_compiler_exe PROPERTIES OUTPUT_NAME "mlc")

# Install targets
install(TARGETS ml_compiler_exe
        RUNTIME DESTINATION bin)

# Install headers
install(FILES 
        compiler/compiler.h
        compiler/ast_visitor.h
        compiler/code_generator.h
        compiler/runtime.h
        DESTINATION include/ml_compiler)
