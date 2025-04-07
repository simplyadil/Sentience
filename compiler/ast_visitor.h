#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

// Forward declarations of AST node types from the interpreter
namespace core {
    class NumberNode;
    class StringNode;
    class ListNode;
    class VarAccessNode;
    class VarAssignNode;
    class BinOpNode;
    class UnaryOpNode;
    class IfNode;
    class ForNode;
    class WhileNode;
    class FuncDefNode;
    class CallNode;
    class ReturnNode;
    class ContinueNode;
    class BreakNode;
    class EmbedNode;
    class AICallNode;
    class PipeNode;
}

namespace ml_compiler {

// Result of code generation for a node
class CodeGenResult {
public:
    CodeGenResult() : success_(true) {}
    CodeGenResult(const std::string& error) : success_(false), error_(error) {}

    bool isSuccess() const { return success_; }
    const std::string& getError() const { return error_; }
    const std::string& getCode() const { return code_; }
    void setCode(const std::string& code) { code_ = code; }
    
    // Type information for the generated code
    void setType(const std::string& type) { type_ = type; }
    const std::string& getType() const { return type_; }

private:
    bool success_;
    std::string error_;
    std::string code_;
    std::string type_;
};

// Symbol table for tracking variables during compilation
class SymbolTable {
public:
    SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr) : parent_(parent) {}

    void define(const std::string& name, const std::string& type);
    bool isDefined(const std::string& name) const;
    std::string getType(const std::string& name) const;
    
    std::shared_ptr<SymbolTable> createChildScope();

private:
    std::unordered_map<std::string, std::string> symbols_;
    std::shared_ptr<SymbolTable> parent_;
};

// Visitor for AST nodes to generate C++ code
class ASTVisitor {
public:
    ASTVisitor();
    ~ASTVisitor();

    // Visit methods for each node type
    CodeGenResult visitNumberNode(const core::NumberNode* node);
    CodeGenResult visitStringNode(const core::StringNode* node);
    CodeGenResult visitListNode(const core::ListNode* node);
    CodeGenResult visitVarAccessNode(const core::VarAccessNode* node);
    CodeGenResult visitVarAssignNode(const core::VarAssignNode* node);
    CodeGenResult visitBinOpNode(const core::BinOpNode* node);
    CodeGenResult visitUnaryOpNode(const core::UnaryOpNode* node);
    CodeGenResult visitIfNode(const core::IfNode* node);
    CodeGenResult visitForNode(const core::ForNode* node);
    CodeGenResult visitWhileNode(const core::WhileNode* node);
    CodeGenResult visitFuncDefNode(const core::FuncDefNode* node);
    CodeGenResult visitCallNode(const core::CallNode* node);
    CodeGenResult visitReturnNode(const core::ReturnNode* node);
    CodeGenResult visitContinueNode(const core::ContinueNode* node);
    CodeGenResult visitBreakNode(const core::BreakNode* node);
    CodeGenResult visitEmbedNode(const core::EmbedNode* node);
    CodeGenResult visitAICallNode(const core::AICallNode* node);
    CodeGenResult visitPipeNode(const core::PipeNode* node);

    // Get the complete generated code
    std::string getGeneratedCode() const;

private:
    // Current symbol table
    std::shared_ptr<SymbolTable> currentScope_;
    
    // Generated code sections
    std::string headerCode_;
    std::string functionDeclarations_;
    std::string functionDefinitions_;
    std::string mainCode_;
    
    // Helper methods
    void enterScope();
    void exitScope();
    std::string generateUniqueVarName(const std::string& prefix);
    
    // Counter for generating unique variable names
    int uniqueCounter_;
};

} // namespace ml_compiler

#endif // AST_VISITOR_H
