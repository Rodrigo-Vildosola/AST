#include "expression/sin_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/cos_node.h"

namespace Expression {

SinNode::SinNode(Node* operand)
    : UnaryOpNode(operand) {}

SinNode::~SinNode() {}

double SinNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::sin(opVal);
    Trace::addTransformation("Evaluating SinNode", toString(), std::to_string(result));
    return result;
}

std::string SinNode::toString() const {
    return "sin(" + operand->toString() + ")";
}

// **Simplification**
Node* SinNode::simplify() const {
    std::string before = toString();
    Node* simplified = new SinNode(operand->simplify());
    Trace::addTransformation("Simplify SinNode", before, simplified->toString());
    return simplified;
}

// **Differentiation (d/dx sin(x) = cos(x) * dx)**
Node* SinNode::derivative(const std::string& variable) const {
    std::string before = toString();
    Node* derivativeResult = new MultiplicationNode(new CosNode(operand->clone()), operand->derivative(variable));
    Trace::addTransformation("Differentiate SinNode", before, derivativeResult->toString());
    return derivativeResult;
}

// **Substitution**
Node* SinNode::substitute(const std::string& variable, Node* value) const {
    std::string before = toString();
    Node* substituted = new SinNode(operand->substitute(variable, value));
    Trace::addTransformation("Substituting in SinNode", before, substituted->toString());
    return substituted;
}

// **Clone**
Node* SinNode::clone() const {
    return new SinNode(operand->clone());
}

} // namespace Expression
