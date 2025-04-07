#include "compiler/compiler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

void printUsage() {
    std::cout << "Usage: compiler [options] <input_file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -o <output_file>   Specify output file (default: a.out)" << std::endl;
    std::cout << "  -c                 Compile only, don't link" << std::endl;
    std::cout << "  -O<level>          Set optimization level (0-3)" << std::endl;
    std::cout << "  -g                 Include debug information" << std::endl;
    std::cout << "  -I<path>           Add include path" << std::endl;
    std::cout << "  -L<path>           Add library path" << std::endl;
    std::cout << "  -l<library>        Link with library" << std::endl;
    std::cout << "  -h, --help         Display this help message" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    // Parse command line arguments
    std::string inputFile;
    std::string outputFile = "a.out";
    bool compileOnly = false;
    int optimizationLevel = 0;
    bool debugInfo = false;
    std::vector<std::string> includePaths;
    std::vector<std::string> libraryPaths;
    std::vector<std::string> libraries;
    
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    outputFile = argv[++i];
                } else {
                    std::cerr << "Error: -o option requires an argument" << std::endl;
                    return 1;
                }
            } else if (strcmp(argv[i], "-c") == 0) {
                compileOnly = true;
            } else if (strncmp(argv[i], "-O", 2) == 0) {
                optimizationLevel = atoi(argv[i] + 2);
            } else if (strcmp(argv[i], "-g") == 0) {
                debugInfo = true;
            } else if (strncmp(argv[i], "-I", 2) == 0) {
                includePaths.push_back(argv[i] + 2);
            } else if (strncmp(argv[i], "-L", 2) == 0) {
                libraryPaths.push_back(argv[i] + 2);
            } else if (strncmp(argv[i], "-l", 2) == 0) {
                libraries.push_back(argv[i] + 2);
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printUsage();
                return 0;
            } else {
                std::cerr << "Error: Unknown option: " << argv[i] << std::endl;
                return 1;
            }
        } else {
            inputFile = argv[i];
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        return 1;
    }
    
    // Read input file
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Failed to open input file: " << inputFile << std::endl;
        return 1;
    }
    
    std::string source;
    std::string line;
    while (std::getline(inFile, line)) {
        source += line + "\n";
    }
    inFile.close();
    
    // Create compiler instance
    ml_compiler::Compiler compiler;
    
    // Configure compiler
    compiler.setOptimizationLevel(optimizationLevel);
    compiler.enableDebugInfo(debugInfo);
    
    for (const auto& path : includePaths) {
        compiler.addIncludePath(path);
    }
    
    for (const auto& path : libraryPaths) {
        compiler.addLibraryPath(path);
    }
    
    for (const auto& lib : libraries) {
        compiler.addLibrary(lib);
    }
    
    // Compile
    ml_compiler::CompilationResult result;
    if (compileOnly) {
        result = compiler.compileToSource(inputFile, source, outputFile);
    } else {
        result = compiler.compileToExecutable(inputFile, source, outputFile);
    }
    
    if (!result.isSuccess()) {
        std::cerr << "Compilation error: " << result.getError().getMessage() << std::endl;
        if (result.getError().getLine() >= 0) {
            std::cerr << "Line " << result.getError().getLine();
            if (result.getError().getColumn() >= 0) {
                std::cerr << ", Column " << result.getError().getColumn();
            }
            std::cerr << std::endl;
        }
        return 1;
    }
    
    std::cout << "Compilation successful: " << outputFile << std::endl;
    return 0;
}
