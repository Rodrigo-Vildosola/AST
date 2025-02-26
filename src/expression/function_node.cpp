#include "expression/function_node.h"
#include "helpers/node_factory.h"
#include "tracing/trace.h"

namespace Expression {

FunctionNode::FunctionNode(const std::string& name, int expectedArgCount, const std::vector<Node*>& arguments, FunctionCallback callback)
    : name(name), expectedArgCount(expectedArgCount), arguments(arguments), callback(callback)
{
    if (arguments.size() != (size_t)expectedArgCount) {
        throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                 " arguments, but got " + std::to_string(arguments.size()));
    }
}

FunctionNode::~FunctionNode() {
    // no manual delete in full arena
}

double FunctionNode::evaluate(const Env &env) {
    if (arguments.size() != (size_t)expectedArgCount) {
        throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                 " arguments, but got " + std::to_string(arguments.size()));
    }
    std::vector<double> argVals;
    for (auto arg : arguments) {
        argVals.push_back(arg->evaluate(env));
    }
    double result = callback(argVals);
    Trace::addTransformation("Evaluating FunctionNode: " + name, toString(), std::to_string(result));
    return result;
}

std::string FunctionNode::toString() const {
    std::ostringstream oss;
    oss << name << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << arguments[i]->toString();
    }
    oss << ")";
    return oss.str();
}

// **Simplify** 
Node* FunctionNode::simplify(NodeFactory &factory) const {
    std::vector<Node*> newArgs;
    newArgs.reserve(arguments.size());
    for (auto arg : arguments) {
        newArgs.push_back(arg->simplify(factory));
    }
    // create a new FunctionNode with simplified args
    // or you might do factory.func(...)
    Node* simplified = factory.func(name, expectedArgCount, newArgs, callback);
    return simplified;
}

// **Derivative** - not yet implemented
Node* FunctionNode::derivative(const std::string& variable, NodeFactory &factory) const {
    Trace::addTransformation("Derivative of function " + name, toString(), "Unchanged");
    return clone(factory);
}

// **Substitution**
Node* FunctionNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    std::vector<Node*> newArgs;
    newArgs.reserve(arguments.size());
    for (auto arg : arguments) {
        newArgs.push_back(arg->substitute(variable, value, factory));
    }
    return factory.func(name, expectedArgCount, newArgs, callback);
}

// **Clone**
Node* FunctionNode::clone(NodeFactory &factory) const {
    // clone each argument
    std::vector<Node*> clonedArgs;
    clonedArgs.reserve(arguments.size());
    for (auto arg : arguments) {
        clonedArgs.push_back(arg->clone(factory));
    }
    return factory.func(name, expectedArgCount, clonedArgs, callback);
}

} // namespace Expression
