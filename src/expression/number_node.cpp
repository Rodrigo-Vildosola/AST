#include "expression/number_node.h"

namespace Expression {

NumberNode::NumberNode(double value) : value(value) {}

NumberNode::~NumberNode() {}

double NumberNode::evaluate(const Env &env) {
    Trace::add("Evaluating NumberNode: " + toString());
    return value;
}

std::string NumberNode::toString() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

Node* NumberNode::simplify() const {
    return new NumberNode(value);
}

Node* NumberNode::derivative(const std::string& variable) const {
    return new NumberNode(0);  // d/dx (constant) = 0
}

Node* NumberNode::substitute(const std::string& variable, Node* value) const {
    return new NumberNode(this->value);
}

Node* NumberNode::clone() const {
    return new NumberNode(value);
}

double NumberNode::getValue() const {
    return value;
}

} // namespace Expression
