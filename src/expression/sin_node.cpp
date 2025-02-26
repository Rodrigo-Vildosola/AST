#include "expression/sin_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/cos_node.h"  // Assume a CosNode exists

namespace Expression {

SinNode::SinNode(Node* operand)
    : UnaryOpNode(operand) {}

SinNode::~SinNode() {}

double SinNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::sin(opVal);
    Trace::add("Evaluating SinNode: sin(" + operand->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string SinNode::toString() const {
    return "sin(" + operand->toString() + ")";
}

// **Simplification**
Node* SinNode::simplify() const {
    return new SinNode(operand->simplify());
}

// **Differentiation (d/dx sin(x) = cos(x) * dx)**
Node* SinNode::derivative(const std::string& variable) const {
    return new MultiplicationNode(new CosNode(operand->clone()), operand->derivative(variable));
}

// **Substitution**
Node* SinNode::substitute(const std::string& variable, Node* value) const {
    return new SinNode(operand->substitute(variable, value));
}

// **Clone**
Node* SinNode::clone() const {
    return new SinNode(operand->clone());
}

} // namespace Expression
