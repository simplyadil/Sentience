#include "code_generator.h"
#include <sstream>

namespace ml_compiler {

CodeGenerator::CodeGenerator() : indentationLevel_(0) {
}

CodeGenerator::~CodeGenerator() {
}

std::string CodeGenerator::generateProgram(
    const std::string& headerCode,
    const std::string& functionDeclarations,
    const std::string& functionDefinitions,
    const std::string& mainCode
) {
    std::stringstream ss;
    
    ss << headerCode << "\n";
    ss << functionDeclarations << "\n";
    ss << functionDefinitions << "\n";
    ss << mainCode;
    
    return ss.str();
}

std::string CodeGenerator::generateStandardHeaders() {
    return "#include <iostream>\n"
           "#include <vector>\n"
           "#include <string>\n"
           "#include <memory>\n"
           "#include <cmath>\n"
           "#include <functional>\n"
           "#include <unordered_map>\n\n"
           "// ML Runtime Library\n"
           "#include \"runtime.h\"\n\n"
           "using namespace ml_runtime;\n\n";
}

std::string CodeGenerator::generateRuntimeSupport() {
    // This would include any additional runtime support code
    // that's not already in the runtime library
    return "";
}

std::string CodeGenerator::generateTypeDefinitions() {
    // This would include any additional type definitions
    // that are not already in the runtime library
    return "";
}

std::string CodeGenerator::generateFunctionDeclaration(
    const std::string& name,
    const std::string& returnType,
    const std::vector<std::pair<std::string, std::string>>& parameters
) {
    std::stringstream ss;
    
    ss << returnType << " " << name << "(";
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) {
            ss << ", ";
        }
        ss << parameters[i].second << " " << parameters[i].first;
    }
    
    ss << ");\n";
    
    return ss.str();
}

std::string CodeGenerator::generateFunctionDefinition(
    const std::string& name,
    const std::string& returnType,
    const std::vector<std::pair<std::string, std::string>>& parameters,
    const std::string& body
) {
    std::stringstream ss;
    
    ss << returnType << " " << name << "(";
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) {
            ss << ", ";
        }
        ss << parameters[i].second << " " << parameters[i].first;
    }
    
    ss << ") {\n";
    ss << indent(body);
    ss << "}\n";
    
    return ss.str();
}

std::string CodeGenerator::generateVariableDeclaration(
    const std::string& name,
    const std::string& type,
    const std::string& initialValue
) {
    std::stringstream ss;
    
    ss << type << " " << name;
    
    if (!initialValue.empty()) {
        ss << " = " << initialValue;
    }
    
    ss << ";\n";
    
    return ss.str();
}

std::string CodeGenerator::generateIfStatement(
    const std::string& condition,
    const std::string& thenBranch,
    const std::string& elseBranch
) {
    std::stringstream ss;
    
    ss << "if (" << condition << ") {\n";
    ss << indent(thenBranch);
    ss << "}";
    
    if (!elseBranch.empty()) {
        ss << " else {\n";
        ss << indent(elseBranch);
        ss << "}";
    }
    
    ss << "\n";
    
    return ss.str();
}

std::string CodeGenerator::generateForLoop(
    const std::string& init,
    const std::string& condition,
    const std::string& update,
    const std::string& body
) {
    std::stringstream ss;
    
    ss << "for (" << init << "; " << condition << "; " << update << ") {\n";
    ss << indent(body);
    ss << "}\n";
    
    return ss.str();
}

std::string CodeGenerator::generateWhileLoop(
    const std::string& condition,
    const std::string& body
) {
    std::stringstream ss;
    
    ss << "while (" << condition << ") {\n";
    ss << indent(body);
    ss << "}\n";
    
    return ss.str();
}

std::string CodeGenerator::generateFunctionCall(
    const std::string& name,
    const std::vector<std::string>& arguments
) {
    std::stringstream ss;
    
    ss << name << "(";
    
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) {
            ss << ", ";
        }
        ss << arguments[i];
    }
    
    ss << ")";
    
    return ss.str();
}

std::string CodeGenerator::generateReturnStatement(const std::string& value) {
    if (value.empty()) {
        return "return;\n";
    } else {
        return "return " + value + ";\n";
    }
}

std::string CodeGenerator::generateTensorOperation(
    const std::string& operation,
    const std::vector<std::string>& operands
) {
    // This is a placeholder for tensor operations
    // In a real implementation, this would generate code for tensor operations
    // using a library like Eigen or TensorFlow C++ API
    std::stringstream ss;
    
    ss << "/* Tensor operation: " << operation << " */\n";
    ss << "createNumber(0.0)";
    
    return ss.str();
}

std::string CodeGenerator::generateEmbeddingCode(
    const std::string& text,
    const std::string& model
) {
    // This is a placeholder for embedding code
    // In a real implementation, this would generate code for embedding operations
    std::stringstream ss;
    
    ss << "/* Embedding operation with model: " << model << " */\n";
    ss << "embed({createString(" << text << "), createString(\"" << model << "\")})";
    
    return ss.str();
}

std::string CodeGenerator::generateAIModelCall(
    const std::string& model,
    const std::vector<std::string>& arguments
) {
    // This is a placeholder for AI model call code
    // In a real implementation, this would generate code for AI model calls
    std::stringstream ss;
    
    ss << "/* AI model call: " << model << " */\n";
    ss << "aiCall({createString(\"" << model << "\")";
    
    for (const auto& arg : arguments) {
        ss << ", " << arg;
    }
    
    ss << "})";
    
    return ss.str();
}

std::string CodeGenerator::indent(const std::string& code) {
    std::stringstream ss;
    std::stringstream codeStream(code);
    std::string line;
    
    while (std::getline(codeStream, line)) {
        ss << indentLine(line) << "\n";
    }
    
    return ss.str();
}

std::string CodeGenerator::indentLine(const std::string& line) {
    if (line.empty()) {
        return "";
    }
    
    return std::string(indentationLevel_ * 4, ' ') + line;
}

} // namespace ml_compiler
