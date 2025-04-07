#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <fstream>

namespace ml_compiler {

class CompilationError {
public:
    CompilationError(const std::string& message, int line = -1, int column = -1)
        : message_(message), line_(line), column_(column) {}

    std::string getMessage() const { return message_; }
    int getLine() const { return line_; }
    int getColumn() const { return column_; }

private:
    std::string message_;
    int line_;
    int column_;
};

class CompilationResult {
public:
    CompilationResult() : success_(true) {}
    CompilationResult(const CompilationError& error) : success_(false), error_(error) {}

    bool isSuccess() const { return success_; }
    const CompilationError& getError() const { return error_; }

private:
    bool success_;
    CompilationError error_;
};

class Compiler {
public:
    Compiler();
    ~Compiler();

    // Compile source code to C++
    CompilationResult compileToSource(const std::string& filename, const std::string& source, 
                                     const std::string& outputFile);
    
    // Compile and build executable
    CompilationResult compileToExecutable(const std::string& filename, const std::string& source, 
                                         const std::string& outputFile);

    // Set compiler options
    void setOptimizationLevel(int level);
    void enableDebugInfo(bool enable);
    void addIncludePath(const std::string& path);
    void addLibraryPath(const std::string& path);
    void addLibrary(const std::string& library);

private:
    // Internal compilation pipeline
    CompilationResult generateCppCode(const std::string& filename, const std::string& source, 
                                     std::string& generatedCode);
    CompilationResult buildExecutable(const std::string& cppFile, const std::string& outputFile);

    // Compiler configuration
    int optimizationLevel_;
    bool debugInfo_;
    std::vector<std::string> includePaths_;
    std::vector<std::string> libraryPaths_;
    std::vector<std::string> libraries_;
};

} // namespace ml_compiler

#endif // COMPILER_H
