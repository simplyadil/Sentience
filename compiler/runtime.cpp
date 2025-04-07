#include "runtime.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

namespace ml_runtime {

// Value implementation
NumberPtr Value::asNumber() const {
    throw std::runtime_error("Cannot convert value to Number");
}

StringPtr Value::asString() const {
    throw std::runtime_error("Cannot convert value to String");
}

ListPtr Value::asList() const {
    throw std::runtime_error("Cannot convert value to List");
}

FunctionPtr Value::asFunction() const {
    throw std::runtime_error("Cannot convert value to Function");
}

ValuePtr Value::addedTo(const ValuePtr& other) const {
    throw std::runtime_error("Addition not supported for this type");
}

ValuePtr Value::subbedBy(const ValuePtr& other) const {
    throw std::runtime_error("Subtraction not supported for this type");
}

ValuePtr Value::multedBy(const ValuePtr& other) const {
    throw std::runtime_error("Multiplication not supported for this type");
}

ValuePtr Value::divedBy(const ValuePtr& other) const {
    throw std::runtime_error("Division not supported for this type");
}

ValuePtr Value::powedBy(const ValuePtr& other) const {
    throw std::runtime_error("Power operation not supported for this type");
}

ValuePtr Value::comparisonEq(const ValuePtr& other) const {
    throw std::runtime_error("Equality comparison not supported for this type");
}

ValuePtr Value::comparisonNe(const ValuePtr& other) const {
    throw std::runtime_error("Inequality comparison not supported for this type");
}

ValuePtr Value::comparisonLt(const ValuePtr& other) const {
    throw std::runtime_error("Less than comparison not supported for this type");
}

ValuePtr Value::comparisonGt(const ValuePtr& other) const {
    throw std::runtime_error("Greater than comparison not supported for this type");
}

ValuePtr Value::comparisonLte(const ValuePtr& other) const {
    throw std::runtime_error("Less than or equal comparison not supported for this type");
}

ValuePtr Value::comparisonGte(const ValuePtr& other) const {
    throw std::runtime_error("Greater than or equal comparison not supported for this type");
}

ValuePtr Value::andedBy(const ValuePtr& other) const {
    throw std::runtime_error("Logical AND not supported for this type");
}

ValuePtr Value::oredBy(const ValuePtr& other) const {
    throw std::runtime_error("Logical OR not supported for this type");
}

ValuePtr Value::notted() const {
    throw std::runtime_error("Logical NOT not supported for this type");
}

// Number implementation
NumberPtr Number::asNumber() const {
    return std::make_shared<Number>(*this);
}

ValuePtr Number::copy() const {
    return std::make_shared<Number>(value_);
}

std::string Number::toString() const {
    return std::to_string(value_);
}

bool Number::isTrue() const {
    return value_ != 0.0;
}

ValuePtr Number::addedTo(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ + other->asNumber()->getValue());
    } else {
        return Value::addedTo(other);
    }
}

ValuePtr Number::subbedBy(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ - other->asNumber()->getValue());
    } else {
        return Value::subbedBy(other);
    }
}

ValuePtr Number::multedBy(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ * other->asNumber()->getValue());
    } else if (other->isString()) {
        // Repeat string
        std::string result;
        std::string str = other->asString()->getValue();
        int count = static_cast<int>(value_);
        for (int i = 0; i < count; ++i) {
            result += str;
        }
        return std::make_shared<String>(result);
    } else {
        return Value::multedBy(other);
    }
}

ValuePtr Number::divedBy(const ValuePtr& other) const {
    if (other->isNumber()) {
        double otherValue = other->asNumber()->getValue();
        if (otherValue == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        return std::make_shared<Number>(value_ / otherValue);
    } else {
        return Value::divedBy(other);
    }
}

ValuePtr Number::powedBy(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(std::pow(value_, other->asNumber()->getValue()));
    } else {
        return Value::powedBy(other);
    }
}

ValuePtr Number::comparisonEq(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ == other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return std::make_shared<Number>(0.0);
    }
}

ValuePtr Number::comparisonNe(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ != other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return std::make_shared<Number>(1.0);
    }
}

ValuePtr Number::comparisonLt(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ < other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return Value::comparisonLt(other);
    }
}

ValuePtr Number::comparisonGt(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ > other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return Value::comparisonGt(other);
    }
}

ValuePtr Number::comparisonLte(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ <= other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return Value::comparisonLte(other);
    }
}

ValuePtr Number::comparisonGte(const ValuePtr& other) const {
    if (other->isNumber()) {
        return std::make_shared<Number>(value_ >= other->asNumber()->getValue() ? 1.0 : 0.0);
    } else {
        return Value::comparisonGte(other);
    }
}

ValuePtr Number::andedBy(const ValuePtr& other) const {
    return std::make_shared<Number>(isTrue() && other->isTrue() ? 1.0 : 0.0);
}

ValuePtr Number::oredBy(const ValuePtr& other) const {
    return std::make_shared<Number>(isTrue() || other->isTrue() ? 1.0 : 0.0);
}

ValuePtr Number::notted() const {
    return std::make_shared<Number>(isTrue() ? 0.0 : 1.0);
}

NumberPtr Number::null() {
    static NumberPtr instance = std::make_shared<Number>(0.0);
    return instance;
}

NumberPtr Number::trueValue() {
    static NumberPtr instance = std::make_shared<Number>(1.0);
    return instance;
}

NumberPtr Number::falseValue() {
    static NumberPtr instance = std::make_shared<Number>(0.0);
    return instance;
}

NumberPtr Number::pi() {
    static NumberPtr instance = std::make_shared<Number>(3.14159265358979323846);
    return instance;
}

// String implementation
StringPtr String::asString() const {
    return std::make_shared<String>(*this);
}

ValuePtr String::copy() const {
    return std::make_shared<String>(value_);
}

std::string String::toString() const {
    return value_;
}

bool String::isTrue() const {
    return !value_.empty();
}

ValuePtr String::addedTo(const ValuePtr& other) const {
    if (other->isString()) {
        return std::make_shared<String>(value_ + other->asString()->getValue());
    } else {
        return std::make_shared<String>(value_ + other->toString());
    }
}

ValuePtr String::multedBy(const ValuePtr& other) const {
    if (other->isNumber()) {
        std::string result;
        int count = static_cast<int>(other->asNumber()->getValue());
        for (int i = 0; i < count; ++i) {
            result += value_;
        }
        return std::make_shared<String>(result);
    } else {
        return Value::multedBy(other);
    }
}

// List implementation
ListPtr List::asList() const {
    return std::make_shared<List>(*this);
}

ValuePtr List::copy() const {
    std::vector<ValuePtr> copiedElements;
    for (const auto& element : elements_) {
        copiedElements.push_back(element->copy());
    }
    return std::make_shared<List>(copiedElements);
}

std::string List::toString() const {
    std::string result = "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += elements_[i]->toString();
    }
    result += "]";
    return result;
}

bool List::isTrue() const {
    return !elements_.empty();
}

ValuePtr List::getItem(int index) const {
    if (index < 0) {
        index += elements_.size();
    }
    
    if (index < 0 || static_cast<size_t>(index) >= elements_.size()) {
        throw std::runtime_error("List index out of range");
    }
    
    return elements_[index];
}

void List::setItem(int index, const ValuePtr& value) {
    if (index < 0) {
        index += elements_.size();
    }
    
    if (index < 0 || static_cast<size_t>(index) >= elements_.size()) {
        throw std::runtime_error("List index out of range");
    }
    
    elements_[index] = value;
}

void List::append(const ValuePtr& value) {
    elements_.push_back(value);
}

int List::length() const {
    return static_cast<int>(elements_.size());
}

// Function implementation
FunctionPtr Function::asFunction() const {
    return std::make_shared<Function>(*this);
}

ValuePtr Function::copy() const {
    return std::make_shared<Function>(name_, function_);
}

std::string Function::toString() const {
    return "<function " + name_ + ">";
}

bool Function::isTrue() const {
    return true;
}

ValuePtr Function::call(const std::vector<ValuePtr>& args) const {
    return function_(args);
}

// Context implementation
void Context::setVariable(const std::string& name, const ValuePtr& value) {
    variables_[name] = value;
}

ValuePtr Context::getVariable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    
    if (parent_) {
        return parent_->getVariable(name);
    }
    
    throw std::runtime_error("Variable '" + name + "' not defined");
}

bool Context::hasVariable(const std::string& name) const {
    if (variables_.find(name) != variables_.end()) {
        return true;
    }
    
    if (parent_) {
        return parent_->hasVariable(name);
    }
    
    return false;
}

std::shared_ptr<Context> Context::createChild(const std::string& name) {
    return std::make_shared<Context>(name, shared_from_this());
}

// Factory functions
NumberPtr createNumber(double value) {
    return std::make_shared<Number>(value);
}

StringPtr createString(const std::string& value) {
    return std::make_shared<String>(value);
}

ListPtr createList(const std::vector<ValuePtr>& elements) {
    return std::make_shared<List>(elements);
}

FunctionPtr createFunction(const std::string& name, const Function::NativeFunction& function) {
    return std::make_shared<Function>(name, function);
}

// Standard library functions
ValuePtr print(const std::vector<ValuePtr>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << args[i]->toString();
    }
    std::cout << std::endl;
    return Number::null();
}

ValuePtr input(const std::vector<ValuePtr>& args) {
    if (!args.empty()) {
        std::cout << args[0]->toString();
    }
    
    std::string input;
    std::getline(std::cin, input);
    return std::make_shared<String>(input);
}

ValuePtr len(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        throw std::runtime_error("len() requires at least one argument");
    }
    
    if (args[0]->isList()) {
        return std::make_shared<Number>(args[0]->asList()->length());
    } else if (args[0]->isString()) {
        return std::make_shared<Number>(args[0]->asString()->getValue().length());
    } else {
        throw std::runtime_error("len() requires a list or string argument");
    }
}

ValuePtr append(const std::vector<ValuePtr>& args) {
    if (args.size() < 2) {
        throw std::runtime_error("append() requires at least two arguments");
    }
    
    if (!args[0]->isList()) {
        throw std::runtime_error("First argument to append() must be a list");
    }
    
    ListPtr list = args[0]->asList();
    for (size_t i = 1; i < args.size(); ++i) {
        list->append(args[i]);
    }
    
    return list;
}

ValuePtr pop(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        throw std::runtime_error("pop() requires at least one argument");
    }
    
    if (!args[0]->isList()) {
        throw std::runtime_error("First argument to pop() must be a list");
    }
    
    ListPtr list = args[0]->asList();
    if (list->length() == 0) {
        throw std::runtime_error("Cannot pop from an empty list");
    }
    
    int index = -1;
    if (args.size() > 1 && args[1]->isNumber()) {
        index = static_cast<int>(args[1]->asNumber()->getValue());
    }
    
    ValuePtr result = list->getItem(index);
    // TODO: Remove the item from the list
    return result;
}

ValuePtr str(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        return std::make_shared<String>("");
    }
    
    return std::make_shared<String>(args[0]->toString());
}

ValuePtr num(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        return std::make_shared<Number>(0.0);
    }
    
    if (args[0]->isNumber()) {
        return args[0];
    } else if (args[0]->isString()) {
        try {
            double value = std::stod(args[0]->asString()->getValue());
            return std::make_shared<Number>(value);
        } catch (const std::exception&) {
            throw std::runtime_error("Cannot convert string to number");
        }
    } else {
        throw std::runtime_error("Cannot convert to number");
    }
}

// ML-specific functions
ValuePtr embed(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        throw std::runtime_error("embed() requires at least one argument");
    }
    
    if (!args[0]->isString()) {
        throw std::runtime_error("First argument to embed() must be a string");
    }
    
    std::string model = "default";
    if (args.size() > 1 && args[1]->isString()) {
        model = args[1]->asString()->getValue();
    }
    
    // This is a placeholder for embedding functionality
    // In a real implementation, this would call an embedding model
    std::cout << "Embedding text with model: " << model << std::endl;
    
    // Return a dummy embedding as a list of numbers
    std::vector<ValuePtr> embedding;
    for (int i = 0; i < 10; ++i) {
        embedding.push_back(std::make_shared<Number>(static_cast<double>(i) / 10.0));
    }
    
    return std::make_shared<List>(embedding);
}

ValuePtr aiCall(const std::vector<ValuePtr>& args) {
    if (args.empty()) {
        throw std::runtime_error("ai() requires at least one argument");
    }
    
    if (!args[0]->isString()) {
        throw std::runtime_error("First argument to ai() must be a string");
    }
    
    std::string model = args[0]->asString()->getValue();
    
    // This is a placeholder for AI model call functionality
    // In a real implementation, this would call an AI model
    std::cout << "Calling AI model: " << model << std::endl;
    
    // Return a dummy result
    return std::make_shared<String>("AI model response");
}

// Initialize the runtime
void initializeRuntime() {
    // This function would initialize the runtime environment
    // including setting up global variables and functions
    std::cout << "Initializing ML runtime..." << std::endl;
}

} // namespace ml_runtime
