#include "compiler.h"
#include "ast_visitor.h"
#include "code_generator.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

// Forward declarations for the parser and lexer
namespace core {
    class Lexer;
    class Parser;
    class Token;
}

namespace ml_compiler {

Compiler::Compiler() 
    : optimizationLevel_(0), debugInfo_(false) {
}

Compiler::~Compiler() {
}

CompilationResult Compiler::compileToSource(const std::string& filename, 
                                           const std::string& source,
                                           const std::string& outputFile) {
    // Generate C++ code
    std::string generatedCode;
    CompilationResult result = generateCppCode(filename, source, generatedCode);
    if (!result.isSuccess()) {
        return result;
    }
    
    // Write the generated code to the output file
    std::ofstream outFile(outputFile);
    if (!outFile) {
        return CompilationResult(CompilationError("Failed to open output file: " + outputFile));
    }
    
    outFile << generatedCode;
    outFile.close();
    
    return CompilationResult();
}

CompilationResult Compiler::compileToExecutable(const std::string& filename, 
                                               const std::string& source,
                                               const std::string& outputFile) {
    // First, compile to C++ source
    std::string cppFile = outputFile + ".cpp";
    CompilationResult result = compileToSource(filename, source, cppFile);
    if (!result.isSuccess()) {
        return result;
    }
    
    // Then build the executable
    return buildExecutable(cppFile, outputFile);
}

void Compiler::setOptimizationLevel(int level) {
    optimizationLevel_ = level;
}

void Compiler::enableDebugInfo(bool enable) {
    debugInfo_ = enable;
}

void Compiler::addIncludePath(const std::string& path) {
    includePaths_.push_back(path);
}

void Compiler::addLibraryPath(const std::string& path) {
    libraryPaths_.push_back(path);
}

void Compiler::addLibrary(const std::string& library) {
    libraries_.push_back(library);
}

CompilationResult Compiler::generateCppCode(const std::string& filename, 
                                           const std::string& source,
                                           std::string& generatedCode) {
    // This is a placeholder implementation
    // In a real implementation, we would:
    // 1. Use the lexer to tokenize the source
    // 2. Use the parser to build an AST
    // 3. Use the ASTVisitor to generate C++ code
    
    // For now, we'll just generate a simple C++ program
    std::stringstream ss;
    ss << "#include <iostream>\n";
    ss << "#include <vector>\n";
    ss << "#include <string>\n";
    ss << "#include <memory>\n";
    ss << "#include <cmath>\n\n";
    ss << "// Generated from " << filename << "\n\n";
    ss << "int main() {\n";
    ss << "    std::cout << \"Hello from compiled ML program!\" << std::endl;\n";
    ss << "    return 0;\n";
    ss << "}\n";
    
    generatedCode = ss.str();
    return CompilationResult();
}

CompilationResult Compiler::buildExecutable(const std::string& cppFile, 
                                           const std::string& outputFile) {
    // Build the command line for invoking the C++ compiler
    std::stringstream cmdSS;
    
    // Use g++ or clang++ depending on availability
    cmdSS << "g++ ";
    
    // Add optimization level
    if (optimizationLevel_ > 0) {
        cmdSS << "-O" << optimizationLevel_ << " ";
    }
    
    // Add debug info
    if (debugInfo_) {
        cmdSS << "-g ";
    }
    
    // Add include paths
    for (const auto& path : includePaths_) {
        cmdSS << "-I\"" << path << "\" ";
    }
    
    // Add library paths
    for (const auto& path : libraryPaths_) {
        cmdSS << "-L\"" << path << "\" ";
    }
    
    // Add input file and output file
    cmdSS << "\"" << cppFile << "\" -o \"" << outputFile << "\" ";
    
    // Add libraries
    for (const auto& lib : libraries_) {
        cmdSS << "-l" << lib << " ";
    }
    
    // Execute the command
    std::string cmd = cmdSS.str();
    int result = std::system(cmd.c_str());
    
    if (result != 0) {
        return CompilationResult(CompilationError("C++ compilation failed with error code: " + 
                                                 std::to_string(result)));
    }
    
    return CompilationResult();
}

} // namespace ml_compiler
