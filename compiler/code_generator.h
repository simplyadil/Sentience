#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ml_compiler {

// Class for generating C++ code
class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();

    // Generate the complete C++ program
    std::string generateProgram(
        const std::string& headerCode,
        const std::string& functionDeclarations,
        const std::string& functionDefinitions,
        const std::string& mainCode
    );

    // Generate standard headers and runtime includes
    std::string generateStandardHeaders();
    
    // Generate runtime support code
    std::string generateRuntimeSupport();
    
    // Generate type definitions
    std::string generateTypeDefinitions();
    
    // Helper methods for common code patterns
    std::string generateFunctionDeclaration(
        const std::string& name,
        const std::string& returnType,
        const std::vector<std::pair<std::string, std::string>>& parameters
    );
    
    std::string generateFunctionDefinition(
        const std::string& name,
        const std::string& returnType,
        const std::vector<std::pair<std::string, std::string>>& parameters,
        const std::string& body
    );
    
    std::string generateVariableDeclaration(
        const std::string& name,
        const std::string& type,
        const std::string& initialValue = ""
    );
    
    std::string generateIfStatement(
        const std::string& condition,
        const std::string& thenBranch,
        const std::string& elseBranch = ""
    );
    
    std::string generateForLoop(
        const std::string& init,
        const std::string& condition,
        const std::string& update,
        const std::string& body
    );
    
    std::string generateWhileLoop(
        const std::string& condition,
        const std::string& body
    );
    
    std::string generateFunctionCall(
        const std::string& name,
        const std::vector<std::string>& arguments
    );
    
    std::string generateReturnStatement(const std::string& value = "");
    
    // ML-specific code generation
    std::string generateTensorOperation(
        const std::string& operation,
        const std::vector<std::string>& operands
    );
    
    std::string generateEmbeddingCode(
        const std::string& text,
        const std::string& model = "default"
    );
    
    std::string generateAIModelCall(
        const std::string& model,
        const std::vector<std::string>& arguments
    );

private:
    // Indentation level for pretty-printing
    int indentationLevel_;
    
    // Helper methods
    std::string indent(const std::string& code);
    std::string indentLine(const std::string& line);
};

} // namespace ml_compiler

#endif // CODE_GENERATOR_H
