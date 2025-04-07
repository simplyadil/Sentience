# ML Language Compiler

A domain-specific programming language for machine learning applications with a complete compiler pipeline including lexer, parser, and interpreter in C++ and Python.

## Features

- Custom language syntax designed for machine learning tasks
- Lexer and parser for analyzing source code
- Interpreter for executing code dynamically
- Compiler for generating optimized C++ code
- Support for tensor operations and automatic differentiation
- Integration with AI models and embedding operations
- Standard library with data processing and visualization functions

## Architecture

The ML Language system consists of several components:

1. **Lexer**: Tokenizes source code into a stream of tokens
2. **Parser**: Builds an abstract syntax tree (AST) from tokens
3. **Interpreter**: Executes the AST directly for dynamic execution
4. **Compiler**: Translates the AST to C++ code for optimized performance
5. **Runtime Library**: Provides support functions and data structures

## Building the Compiler

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Installation

```bash
cmake --install .
```

## Usage

### Compiling a Program

```bash
mlc -o output_file input_file.ml
```

### Command Line Options

- `-o <output_file>`: Specify output file (default: a.out)
- `-c`: Compile only, don't link
- `-O<level>`: Set optimization level (0-3)
- `-g`: Include debug information
- `-I<path>`: Add include path
- `-L<path>`: Add library path
- `-l<library>`: Link with library
- `-h, --help`: Display help message

## Language Syntax

The ML Language supports a variety of programming constructs:

```
# Variables and basic operations
var x = 10
var y = 20
var z = x + y

# Functions
fun add(a, b)
    return a + b
end

# Control flow
if x > 10 then
    print("x is greater than 10")
elif x < 10 then
    print("x is less than 10")
else
    print("x is equal to 10")
end

# Loops
for i = 0 to 10 step 1 then
    print(i)
end

while x > 0 then
    x = x - 1
end

# Lists
var list = [1, 2, 3, 4, 5]
print(list[0])

# ML-specific features
var embedding = EMBED "This is a text to embed" WITH "model_name"
var result = AI model_name("input", param1, param2)
var processed = data | preprocess | model | postprocess
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
