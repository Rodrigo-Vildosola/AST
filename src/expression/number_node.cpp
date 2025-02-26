#include "expression/number_node.h"
#include "helpers/node_factory.h"  // NodeFactory
#include "tracing/trace.h"
#include <sstream>

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

// **Simplify**
Node* NumberNode::simplify(NodeFactory &factory) const {
    // A number is already simplified, so we can just return `this` or clone it.
    // For consistency, let’s just return `this`.
    // If you prefer returning a new node, do: return factory.num(value);
    return const_cast<NumberNode*>(this); 
}

// **Derivative**
Node* NumberNode::derivative(const std::string& variable, NodeFactory &factory) const {
    // Derivative of a constant is 0
    return factory.num(0);
}

// **Substitute**
Node* NumberNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    // A number is unaffected by substitution
    return const_cast<NumberNode*>(this); 
}

// **Clone**
Node* NumberNode::clone(NodeFactory &factory) const {
    return factory.num(value);
}

double NumberNode::getValue() const {
    return value;
}

} // namespace Expression
