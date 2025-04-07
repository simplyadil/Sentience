#include "ast_visitor.h"
#include <sstream>

namespace ml_compiler {

// SymbolTable implementation
void SymbolTable::define(const std::string& name, const std::string& type) {
    symbols_[name] = type;
}

bool SymbolTable::isDefined(const std::string& name) const {
    if (symbols_.find(name) != symbols_.end()) {
        return true;
    }
    
    if (parent_) {
        return parent_->isDefined(name);
    }
    
    return false;
}

std::string SymbolTable::getType(const std::string& name) const {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second;
    }
    
    if (parent_) {
        return parent_->getType(name);
    }
    
    return "unknown";
}

std::shared_ptr<SymbolTable> SymbolTable::createChildScope() {
    return std::make_shared<SymbolTable>(shared_from_this());
}

// ASTVisitor implementation
ASTVisitor::ASTVisitor() 
    : currentScope_(std::make_shared<SymbolTable>()), uniqueCounter_(0) {
    // Initialize with standard headers and runtime support
    headerCode_ = "#include <iostream>\n"
                  "#include <vector>\n"
                  "#include <string>\n"
                  "#include <memory>\n"
                  "#include <cmath>\n"
                  "#include <functional>\n"
                  "#include <unordered_map>\n\n"
                  "// ML Runtime Library\n"
                  "#include \"runtime.h\"\n\n"
                  "using namespace ml_runtime;\n\n";
    
    // Initialize main function
    mainCode_ = "int main(int argc, char** argv) {\n"
                "    // Initialize runtime\n"
                "    initializeRuntime();\n\n"
                "    // Create global context\n"
                "    auto globalContext = std::make_shared<Context>(\"global\");\n\n";
}

ASTVisitor::~ASTVisitor() {
}

std::string ASTVisitor::getGeneratedCode() const {
    std::stringstream ss;
    
    // Add header code
    ss << headerCode_;
    
    // Add function declarations
    ss << "// Function declarations\n";
    ss << functionDeclarations_;
    ss << "\n";
    
    // Add function definitions
    ss << "// Function definitions\n";
    ss << functionDefinitions_;
    ss << "\n";
    
    // Add main function
    ss << "// Main function\n";
    ss << mainCode_;
    
    // Close main function
    ss << "    return 0;\n";
    ss << "}\n";
    
    return ss.str();
}

void ASTVisitor::enterScope() {
    currentScope_ = currentScope_->createChildScope();
}

void ASTVisitor::exitScope() {
    // TODO: Implement proper scope exit
}

std::string ASTVisitor::generateUniqueVarName(const std::string& prefix) {
    return prefix + "_" + std::to_string(uniqueCounter_++);
}

// Placeholder implementations for visit methods
// In a real implementation, these would generate actual C++ code based on the AST nodes

CodeGenResult ASTVisitor::visitNumberNode(const core::NumberNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitStringNode(const core::StringNode* node) {
    CodeGenResult result;
    result.setCode("createString(\"\")"); // Placeholder
    result.setType("String");
    return result;
}

CodeGenResult ASTVisitor::visitListNode(const core::ListNode* node) {
    CodeGenResult result;
    result.setCode("createList({})"); // Placeholder
    result.setType("List");
    return result;
}

CodeGenResult ASTVisitor::visitVarAccessNode(const core::VarAccessNode* node) {
    CodeGenResult result;
    result.setCode("globalContext->getVariable(\"\")"); // Placeholder
    result.setType("Value");
    return result;
}

CodeGenResult ASTVisitor::visitVarAssignNode(const core::VarAssignNode* node) {
    CodeGenResult result;
    result.setCode("globalContext->setVariable(\"\", createNumber(0.0))"); // Placeholder
    result.setType("Value");
    return result;
}

CodeGenResult ASTVisitor::visitBinOpNode(const core::BinOpNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitUnaryOpNode(const core::UnaryOpNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitIfNode(const core::IfNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitForNode(const core::ForNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitWhileNode(const core::WhileNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Number");
    return result;
}

CodeGenResult ASTVisitor::visitFuncDefNode(const core::FuncDefNode* node) {
    CodeGenResult result;
    result.setCode("createFunction(\"\", [](const std::vector<ValuePtr>& args) { return createNumber(0.0); })"); // Placeholder
    result.setType("Function");
    return result;
}

CodeGenResult ASTVisitor::visitCallNode(const core::CallNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Value");
    return result;
}

CodeGenResult ASTVisitor::visitReturnNode(const core::ReturnNode* node) {
    CodeGenResult result;
    result.setCode("return createNumber(0.0)"); // Placeholder
    result.setType("void");
    return result;
}

CodeGenResult ASTVisitor::visitContinueNode(const core::ContinueNode* node) {
    CodeGenResult result;
    result.setCode("continue"); // Placeholder
    result.setType("void");
    return result;
}

CodeGenResult ASTVisitor::visitBreakNode(const core::BreakNode* node) {
    CodeGenResult result;
    result.setCode("break"); // Placeholder
    result.setType("void");
    return result;
}

CodeGenResult ASTVisitor::visitEmbedNode(const core::EmbedNode* node) {
    CodeGenResult result;
    result.setCode("embed({createString(\"\")})"); // Placeholder
    result.setType("Value");
    return result;
}

CodeGenResult ASTVisitor::visitAICallNode(const core::AICallNode* node) {
    CodeGenResult result;
    result.setCode("aiCall({createString(\"\")})"); // Placeholder
    result.setType("Value");
    return result;
}

CodeGenResult ASTVisitor::visitPipeNode(const core::PipeNode* node) {
    CodeGenResult result;
    result.setCode("createNumber(0.0)"); // Placeholder
    result.setType("Value");
    return result;
}

} // namespace ml_compiler
