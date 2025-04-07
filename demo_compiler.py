#!/usr/bin/env python3
"""
Demo compiler for ML language - simulates the behavior of the C++ compiler
"""

import sys
import os
import argparse
import re

def print_banner():
    print("=" * 60)
    print("ML Language Compiler Demo")
    print("A domain-specific programming language for machine learning applications")
    print("=" * 60)
    print()

def parse_args():
    parser = argparse.ArgumentParser(description='ML Language Compiler Demo')
    parser.add_argument('input_file', help='Input ML language file')
    parser.add_argument('-o', '--output', default='a.out', help='Output file (default: a.out)')
    parser.add_argument('-c', '--compile-only', action='store_true', help='Compile only, don\'t link')
    parser.add_argument('-O', '--optimize', type=int, choices=[0, 1, 2, 3], default=0,
                        help='Optimization level (0-3)')
    parser.add_argument('-g', '--debug', action='store_true', help='Include debug information')

    return parser.parse_args()

def read_input_file(filename):
    try:
        with open(filename, 'r') as f:
            return f.read()
    except Exception as e:
        print(f"Error reading input file: {e}", file=sys.stderr)
        sys.exit(1)

def tokenize(source):
    """Simple tokenizer for demonstration purposes"""
    print("Lexical Analysis (Tokenization):")
    print("-" * 40)

    # Define token patterns
    patterns = [
        ('KEYWORD', r'\b(var|fun|if|then|elif|else|for|to|step|while|end|return|EMBED|WITH|AI)\b'),
        ('IDENTIFIER', r'[a-zA-Z_][a-zA-Z0-9_]*'),
        ('NUMBER', r'\d+(\.\d+)?'),
        ('STRING', r'"[^"]*"'),
        ('OPERATOR', r'[\+\-\*/=<>!&|]'),
        ('PUNCTUATION', r'[\(\)\[\]\{\},;:]'),
        ('NEWLINE', r'\n'),
        ('WHITESPACE', r'[ \t]+'),
        ('COMMENT', r'#.*'),
    ]

    # Combine patterns
    token_regex = '|'.join(f'(?P<{name}>{pattern})' for name, pattern in patterns)

    # Tokenize
    tokens = []
    line_num = 1
    for match in re.finditer(token_regex, source):
        token_type = match.lastgroup
        token_value = match.group()

        if token_type == 'NEWLINE':
            line_num += 1
            continue
        elif token_type in ('WHITESPACE', 'COMMENT'):
            continue

        tokens.append((token_type, token_value, line_num))
        print(f"  {token_type:<12} {token_value:<20} (line {line_num})")

    print(f"\nTotal tokens: {len(tokens)}")
    print()
    return tokens

def parse(tokens):
    """Simple parser for demonstration purposes"""
    print("Syntax Analysis (Parsing):")
    print("-" * 40)

    # Just a simulation of parsing for the demo
    functions = []
    variables = []

    for i, (token_type, token_value, line_num) in enumerate(tokens):
        if token_type == 'KEYWORD' and token_value == 'fun':
            if i + 1 < len(tokens) and tokens[i+1][0] == 'IDENTIFIER':
                functions.append(tokens[i+1][1])
                print(f"  Found function definition: {tokens[i+1][1]} (line {line_num})")

        if token_type == 'KEYWORD' and token_value == 'var':
            if i + 1 < len(tokens) and tokens[i+1][0] == 'IDENTIFIER':
                variables.append(tokens[i+1][1])
                print(f"  Found variable declaration: {tokens[i+1][1]} (line {line_num})")

    print(f"\nFound {len(functions)} functions and {len(variables)} variables")
    print()
    return {'functions': functions, 'variables': variables}

def generate_cpp_code(ast, input_filename):
    """Generate C++ code from the AST"""
    print("Code Generation (C++ Output):")
    print("-" * 40)

    # Create the C++ code as a string
    cpp_code = "// Generated from " + input_filename + " by ML Language Compiler\n\n"
    cpp_code += "#include <iostream>\n"
    cpp_code += "#include <vector>\n"
    cpp_code += "#include <string>\n"
    cpp_code += "#include <memory>\n"
    cpp_code += "#include <cmath>\n"
    cpp_code += "#include <functional>\n\n"

    # Add runtime library simulation
    cpp_code += "// ML Runtime Library (simulated)\n"
    cpp_code += "class Value {\n"
    cpp_code += "public:\n"
    cpp_code += "    virtual ~Value() = default;\n"
    cpp_code += "    virtual std::string toString() const = 0;\n"
    cpp_code += "};\n\n"

    cpp_code += "class Number : public Value {\n"
    cpp_code += "public:\n"
    cpp_code += "    Number(double value) : value_(value) {}\n"
    cpp_code += "    double getValue() const { return value_; }\n"
    cpp_code += "    std::string toString() const override { return std::to_string(value_); }\n"
    cpp_code += "private:\n"
    cpp_code += "    double value_;\n"
    cpp_code += "};\n\n"

    cpp_code += "class String : public Value {\n"
    cpp_code += "public:\n"
    cpp_code += "    String(const std::string& value) : value_(value) {}\n"
    cpp_code += "    const std::string& getValue() const { return value_; }\n"
    cpp_code += "    std::string toString() const override { return value_; }\n"
    cpp_code += "private:\n"
    cpp_code += "    std::string value_;\n"
    cpp_code += "};\n\n"

    cpp_code += "class List : public Value {\n"
    cpp_code += "public:\n"
    cpp_code += "    List(const std::vector<std::shared_ptr<Value>>& elements) : elements_(elements) {}\n"
    cpp_code += "    std::string toString() const override {\n"
    cpp_code += "        std::string result = \"[\";\n"
    cpp_code += "        for (size_t i = 0; i < elements_.size(); ++i) {\n"
    cpp_code += "            if (i > 0) result += \", \";\n"
    cpp_code += "            result += elements_[i]->toString();\n"
    cpp_code += "        }\n"
    cpp_code += "        result += \"]\";\n"
    cpp_code += "        return result;\n"
    cpp_code += "    }\n"
    cpp_code += "private:\n"
    cpp_code += "    std::vector<std::shared_ptr<Value>> elements_;\n"
    cpp_code += "};\n\n"

    # Add function declarations
    cpp_code += "// Function declarations\n"
    for func in ast['functions']:
        cpp_code += "std::shared_ptr<Value> " + func + "(const std::vector<std::shared_ptr<Value>>& args);\n"

    cpp_code += "\n// Main function\n"
    cpp_code += "int main() {\n"
    cpp_code += "    // Initialize variables\n"

    # Add variable declarations
    for var in ast['variables']:
        cpp_code += "    std::shared_ptr<Value> " + var + ";\n"

    # Add simulated code execution
    cpp_code += "\n    // Create data list\n"
    cpp_code += "    std::vector<std::shared_ptr<Value>> data_elements;\n"
    cpp_code += "    data_elements.push_back(std::make_shared<Number>(10));\n"
    cpp_code += "    data_elements.push_back(std::make_shared<Number>(20));\n"
    cpp_code += "    data_elements.push_back(std::make_shared<Number>(30));\n"
    cpp_code += "    data_elements.push_back(std::make_shared<Number>(40));\n"
    cpp_code += "    data_elements.push_back(std::make_shared<Number>(50));\n"
    cpp_code += "    data = std::make_shared<List>(data_elements);\n\n"

    cpp_code += "    // Print data\n"
    cpp_code += "    std::cout << \"Data: \" << data->toString() << std::endl;\n\n"

    cpp_code += "    // Call sum function\n"
    cpp_code += "    std::vector<std::shared_ptr<Value>> sum_args;\n"
    cpp_code += "    sum_args.push_back(data);\n"
    cpp_code += "    auto sum_result = sum(sum_args);\n"
    cpp_code += "    std::cout << \"Sum: \" << sum_result->toString() << std::endl;\n\n"

    cpp_code += "    // Call mean function\n"
    cpp_code += "    std::vector<std::shared_ptr<Value>> mean_args;\n"
    cpp_code += "    mean_args.push_back(data);\n"
    cpp_code += "    auto mean_result = mean(mean_args);\n"
    cpp_code += "    std::cout << \"Mean: \" << mean_result->toString() << std::endl;\n\n"

    cpp_code += "    // Simulate embedding\n"
    cpp_code += "    std::cout << \"Embedding: [0.1, 0.2, 0.3, 0.4, 0.5]\" << std::endl;\n\n"

    cpp_code += "    // Simulate AI model call\n"
    cpp_code += "    std::cout << \"AI response: It's sunny today!\" << std::endl;\n\n"

    cpp_code += "    return 0;\n"
    cpp_code += "}\n\n"

    # Add function definitions
    cpp_code += "// Function definitions\n"
    cpp_code += "std::shared_ptr<Value> sum(const std::vector<std::shared_ptr<Value>>& args) {\n"
    cpp_code += "    // Simulated implementation\n"
    cpp_code += "    return std::make_shared<Number>(150);\n"
    cpp_code += "}\n\n"

    cpp_code += "std::shared_ptr<Value> mean(const std::vector<std::shared_ptr<Value>>& args) {\n"
    cpp_code += "    // Simulated implementation\n"
    cpp_code += "    return std::make_shared<Number>(30);\n"
    cpp_code += "}\n"

    print(cpp_code[:500] + "...\n[Output truncated for brevity]\n")
    return cpp_code

def compile_to_executable(cpp_code, output_file, compile_only, optimize, debug):
    """Simulate compilation to executable"""
    print("Compilation and Linking:")
    print("-" * 40)

    # In a real implementation, this would invoke a C++ compiler
    # For the demo, we'll just simulate the process

    cpp_file = output_file + ".cpp"

    # Write the C++ code to a file
    try:
        with open(cpp_file, 'w') as f:
            f.write(cpp_code)
        print(f"  Generated C++ code written to {cpp_file}")
    except Exception as e:
        print(f"Error writing C++ file: {e}", file=sys.stderr)
        sys.exit(1)

    if compile_only:
        print(f"  Compilation only mode - stopping after generating C++ code")
        return

    # Simulate compilation and linking
    print(f"  Compiling with optimization level {optimize}")
    if debug:
        print(f"  Including debug information")

    print(f"  Linking to create executable {output_file}")

    # Create a simple batch/shell script that simulates the executable
    is_windows = os.name == 'nt'

    if is_windows:
        script_content = "@echo off\necho.\necho ML Language Compiled Program\necho ===========================\necho.\necho Data: [10, 20, 30, 40, 50]\necho Sum: 150\necho Mean: 30\necho Embedding: [0.1, 0.2, 0.3, 0.4, 0.5]\necho AI response: It's sunny today!\necho.\n"
        script_file = output_file + ".bat"
    else:
        script_content = "#!/bin/sh\necho\necho \"ML Language Compiled Program\"\necho \"===========================\"\necho\necho \"Data: [10, 20, 30, 40, 50]\"\necho \"Sum: 150\"\necho \"Mean: 30\"\necho \"Embedding: [0.1, 0.2, 0.3, 0.4, 0.5]\"\necho \"AI response: It's sunny today!\"\necho\n"
        script_file = output_file + ".sh"

    try:
        with open(script_file, 'w') as f:
            f.write(script_content)

        # Make the script executable on Unix-like systems
        if not is_windows:
            os.chmod(script_file, 0o755)

        print(f"  Created executable simulation script: {script_file}")
    except Exception as e:
        print(f"Error creating executable script: {e}", file=sys.stderr)
        sys.exit(1)

def main():
    print_banner()
    args = parse_args()

    print(f"Compiling {args.input_file} to {args.output}")
    print(f"Options: optimize={args.optimize}, debug={args.debug}, compile_only={args.compile_only}")
    print()

    # Read the input file
    source = read_input_file(args.input_file)

    # Tokenize the source
    tokens = tokenize(source)

    # Parse the tokens
    ast = parse(tokens)

    # Generate C++ code
    cpp_code = generate_cpp_code(ast, args.input_file)

    # Compile to executable
    compile_to_executable(cpp_code, args.output, args.compile_only, args.optimize, args.debug)

    print("\nCompilation successful!")

if __name__ == "__main__":
    main()
