#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <functional>

namespace ml_runtime {

// Forward declarations
class Value;
class Number;
class String;
class List;
class Function;

// Type for runtime values
using ValuePtr = std::shared_ptr<Value>;
using NumberPtr = std::shared_ptr<Number>;
using StringPtr = std::shared_ptr<String>;
using ListPtr = std::shared_ptr<List>;
using FunctionPtr = std::shared_ptr<Function>;

// Base class for all runtime values
class Value {
public:
    virtual ~Value() = default;
    
    // Type checking
    virtual bool isNumber() const { return false; }
    virtual bool isString() const { return false; }
    virtual bool isList() const { return false; }
    virtual bool isFunction() const { return false; }
    
    // Conversion methods
    virtual NumberPtr asNumber() const;
    virtual StringPtr asString() const;
    virtual ListPtr asList() const;
    virtual FunctionPtr asFunction() const;
    
    // Common operations
    virtual ValuePtr copy() const = 0;
    virtual std::string toString() const = 0;
    virtual bool isTrue() const = 0;
    
    // Binary operations
    virtual ValuePtr addedTo(const ValuePtr& other) const;
    virtual ValuePtr subbedBy(const ValuePtr& other) const;
    virtual ValuePtr multedBy(const ValuePtr& other) const;
    virtual ValuePtr divedBy(const ValuePtr& other) const;
    virtual ValuePtr powedBy(const ValuePtr& other) const;
    
    // Comparison operations
    virtual ValuePtr comparisonEq(const ValuePtr& other) const;
    virtual ValuePtr comparisonNe(const ValuePtr& other) const;
    virtual ValuePtr comparisonLt(const ValuePtr& other) const;
    virtual ValuePtr comparisonGt(const ValuePtr& other) const;
    virtual ValuePtr comparisonLte(const ValuePtr& other) const;
    virtual ValuePtr comparisonGte(const ValuePtr& other) const;
    
    // Logical operations
    virtual ValuePtr andedBy(const ValuePtr& other) const;
    virtual ValuePtr oredBy(const ValuePtr& other) const;
    virtual ValuePtr notted() const;
};

// Number value
class Number : public Value {
public:
    Number(double value) : value_(value) {}
    
    double getValue() const { return value_; }
    
    // Type checking
    bool isNumber() const override { return true; }
    
    // Conversion methods
    NumberPtr asNumber() const override;
    
    // Common operations
    ValuePtr copy() const override;
    std::string toString() const override;
    bool isTrue() const override;
    
    // Binary operations
    ValuePtr addedTo(const ValuePtr& other) const override;
    ValuePtr subbedBy(const ValuePtr& other) const override;
    ValuePtr multedBy(const ValuePtr& other) const override;
    ValuePtr divedBy(const ValuePtr& other) const override;
    ValuePtr powedBy(const ValuePtr& other) const override;
    
    // Comparison operations
    ValuePtr comparisonEq(const ValuePtr& other) const override;
    ValuePtr comparisonNe(const ValuePtr& other) const override;
    ValuePtr comparisonLt(const ValuePtr& other) const override;
    ValuePtr comparisonGt(const ValuePtr& other) const override;
    ValuePtr comparisonLte(const ValuePtr& other) const override;
    ValuePtr comparisonGte(const ValuePtr& other) const override;
    
    // Logical operations
    ValuePtr andedBy(const ValuePtr& other) const override;
    ValuePtr oredBy(const ValuePtr& other) const override;
    ValuePtr notted() const override;
    
    // Static values
    static NumberPtr null();
    static NumberPtr trueValue();
    static NumberPtr falseValue();
    static NumberPtr pi();

private:
    double value_;
};

// String value
class String : public Value {
public:
    String(const std::string& value) : value_(value) {}
    
    const std::string& getValue() const { return value_; }
    
    // Type checking
    bool isString() const override { return true; }
    
    // Conversion methods
    StringPtr asString() const override;
    
    // Common operations
    ValuePtr copy() const override;
    std::string toString() const override;
    bool isTrue() const override;
    
    // Binary operations
    ValuePtr addedTo(const ValuePtr& other) const override;
    ValuePtr multedBy(const ValuePtr& other) const override;
    
private:
    std::string value_;
};

// List value
class List : public Value {
public:
    List(const std::vector<ValuePtr>& elements) : elements_(elements) {}
    
    const std::vector<ValuePtr>& getElements() const { return elements_; }
    
    // Type checking
    bool isList() const override { return true; }
    
    // Conversion methods
    ListPtr asList() const override;
    
    // Common operations
    ValuePtr copy() const override;
    std::string toString() const override;
    bool isTrue() const override;
    
    // List-specific operations
    ValuePtr getItem(int index) const;
    void setItem(int index, const ValuePtr& value);
    void append(const ValuePtr& value);
    int length() const;
    
private:
    std::vector<ValuePtr> elements_;
};

// Function value
class Function : public Value {
public:
    using NativeFunction = std::function<ValuePtr(const std::vector<ValuePtr>&)>;
    
    Function(const std::string& name, const NativeFunction& function)
        : name_(name), function_(function) {}
    
    const std::string& getName() const { return name_; }
    
    // Type checking
    bool isFunction() const override { return true; }
    
    // Conversion methods
    FunctionPtr asFunction() const override;
    
    // Common operations
    ValuePtr copy() const override;
    std::string toString() const override;
    bool isTrue() const override;
    
    // Function-specific operations
    ValuePtr call(const std::vector<ValuePtr>& args) const;
    
private:
    std::string name_;
    NativeFunction function_;
};

// Runtime context
class Context {
public:
    Context(const std::string& name, std::shared_ptr<Context> parent = nullptr)
        : name_(name), parent_(parent) {}
    
    const std::string& getName() const { return name_; }
    
    void setVariable(const std::string& name, const ValuePtr& value);
    ValuePtr getVariable(const std::string& name) const;
    bool hasVariable(const std::string& name) const;
    
    std::shared_ptr<Context> createChild(const std::string& name);
    
private:
    std::string name_;
    std::shared_ptr<Context> parent_;
    std::unordered_map<std::string, ValuePtr> variables_;
};

// Runtime error
class RuntimeError {
public:
    RuntimeError(const std::string& message) : message_(message) {}
    
    const std::string& getMessage() const { return message_; }
    
private:
    std::string message_;
};

// Result of a runtime operation
class RuntimeResult {
public:
    RuntimeResult() : success_(true) {}
    RuntimeResult(const RuntimeError& error) : success_(false), error_(error) {}
    RuntimeResult(const ValuePtr& value) : success_(true), value_(value) {}
    
    bool isSuccess() const { return success_; }
    const RuntimeError& getError() const { return error_; }
    const ValuePtr& getValue() const { return value_; }
    
private:
    bool success_;
    RuntimeError error_;
    ValuePtr value_;
};

// Initialize the runtime
void initializeRuntime();

// Create standard values
NumberPtr createNumber(double value);
StringPtr createString(const std::string& value);
ListPtr createList(const std::vector<ValuePtr>& elements);
FunctionPtr createFunction(const std::string& name, const Function::NativeFunction& function);

// Standard library functions
ValuePtr print(const std::vector<ValuePtr>& args);
ValuePtr input(const std::vector<ValuePtr>& args);
ValuePtr len(const std::vector<ValuePtr>& args);
ValuePtr append(const std::vector<ValuePtr>& args);
ValuePtr pop(const std::vector<ValuePtr>& args);
ValuePtr str(const std::vector<ValuePtr>& args);
ValuePtr num(const std::vector<ValuePtr>& args);

// ML-specific functions
ValuePtr embed(const std::vector<ValuePtr>& args);
ValuePtr aiCall(const std::vector<ValuePtr>& args);

} // namespace ml_runtime

#endif // RUNTIME_H
