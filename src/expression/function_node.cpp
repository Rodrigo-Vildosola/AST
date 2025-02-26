#include "expression/function_node.h"
#include "tracing/trace.h"

namespace Expression {

FunctionNode::FunctionNode(const std::string& name, int expectedArgCount, const std::vector<Node*>& arguments, FunctionCallback callback)
    : name(name), expectedArgCount(expectedArgCount), arguments(arguments), callback(callback) {
    if (arguments.size() != static_cast<size_t>(expectedArgCount)) {
         throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                  " arguments, but got " + std::to_string(arguments.size()));
    }
}

FunctionNode::~FunctionNode() {
    // for (auto arg : arguments) {
    //     delete arg;
    // }
}

double FunctionNode::evaluate(const Env &env) {
    if (arguments.size() != static_cast<size_t>(expectedArgCount)) {
         throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                  " arguments, but got " + std::to_string(arguments.size()));
    }
    std::vector<double> argValues;
    for (auto arg : arguments) {
        argValues.push_back(arg->evaluate(env));
    }
    double result = callback(argValues);
    Trace::addTransformation("Evaluating FunctionNode: " + name, toString(), std::to_string(result));
    return result;
}

std::string FunctionNode::toString() const {
    std::ostringstream oss;
    oss << name << "(";
    bool first = true;
    for (auto arg : arguments) {
        if (!first) {
            oss << ", ";
        }
        oss << arg->toString();
        first = false;
    }
    oss << ")";
    return oss.str();
}

// **Simplification**
Node* FunctionNode::simplify() const {
    std::vector<Node*> simplifiedArgs;
    for (auto arg : arguments) {
        simplifiedArgs.push_back(arg->simplify());
    }
    return new FunctionNode(name, expectedArgCount, simplifiedArgs, callback);
}

// **Derivative (not implemented, requires function definitions)**
Node* FunctionNode::derivative(const std::string& variable) const {
    Trace::addTransformation("Derivative of function " + name + " is not implemented", toString(), "Unchanged");
    return clone();
}

// **Substitution**
Node* FunctionNode::substitute(const std::string& variable, Node* value) const {
    std::vector<Node*> substitutedArgs;
    for (auto arg : arguments) {
        substitutedArgs.push_back(arg->substitute(variable, value));
    }
    return new FunctionNode(name, expectedArgCount, substitutedArgs, callback);
}

// **Clone**
Node* FunctionNode::clone() const {
    return new FunctionNode(name, expectedArgCount, arguments, callback);
}

} // namespace Expression
