#include "expression/variable_node.h"
#include "expression/trace.h"
#include <sstream>

namespace Expression {

VariableNode::VariableNode(const std::string& name) : name(name) {}

VariableNode::~VariableNode() {}

double VariableNode::evaluate(const Env &env) {
    // Use the passed environment 'env' instead of a global environment.
    auto it = env.find(name);
    if (it != env.end()) {
        double value = it->second;
        Trace::add("Evaluating VariableNode: " + name + " = " + std::to_string(value));
        return value;
    } else {
        Trace::add("Evaluating VariableNode: " + name + " not found in environment, defaulting to 0");
        return 0.0; // Optionally, you might throw an exception for undefined variables.
    }
}

std::string VariableNode::toString() const {
    return name;
}

} // namespace Expression
