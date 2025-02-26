#include "expression/function_node.h"


namespace Expression {

FunctionNode::FunctionNode(const std::string& name, int expectedArgCount, const std::vector<Node*>& arguments, FunctionCallback callback)
    : name(name), expectedArgCount(expectedArgCount), arguments(arguments), callback(callback) {
    // Immediately validate the argument count.
    if (arguments.size() != static_cast<size_t>(expectedArgCount)) {
         throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                  " arguments, but got " + std::to_string(arguments.size()));
    }
}

FunctionNode::~FunctionNode() {
    for (auto arg : arguments) {
        delete arg;
    }
}

double FunctionNode::evaluate(const Env &env) {
    // Double-check the argument count at evaluation time.
    if (arguments.size() != static_cast<size_t>(expectedArgCount)) {
         throw std::runtime_error("Function " + name + " expects " + std::to_string(expectedArgCount) +
                                  " arguments, but got " + std::to_string(arguments.size()));
    }
    std::vector<double> argValues;
    std::ostringstream argStr;
    bool first = true;
    for (auto arg : arguments) {
        double val = arg->evaluate(env);
        argValues.push_back(val);
        if (!first) {
            argStr << ", ";
        }
        argStr << arg->toString();
        first = false;
    }
    double result = callback(argValues);
    Trace::add("Evaluating FunctionNode: " + name + "(" + argStr.str() +
               ") = " + std::to_string(result));
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

} // namespace Expression
