#include "expression/variable_node.h"
#include "expression/number_node.h"

namespace Expression {

VariableNode::VariableNode(const std::string& name) : name(name) {}

VariableNode::~VariableNode() {}

double VariableNode::evaluate(const Env &env) {
    auto it = env.find(name);
    if (it != env.end()) {
        return it->second;
    }
    return 0.0;  // Default to 0 if not found.
}

std::string VariableNode::toString() const {
    return name;
}

// **Simplification**
Node* VariableNode::simplify() const {
    return new VariableNode(name);
}

// **Differentiation**
Node* VariableNode::derivative(const std::string& variable) const {
    return new NumberNode(name == variable ? 1 : 0);
}

// **Substitution**
Node* VariableNode::substitute(const std::string& variable, Node* value) const {
    return (name == variable) ? value->clone() : new VariableNode(name);
}

// **Clone**
Node* VariableNode::clone() const {
    return new VariableNode(name);
}

} // namespace Expression
