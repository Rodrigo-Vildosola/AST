#include "expression/cos_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/sin_node.h"

namespace Expression {

CosNode::CosNode(Node* operand)
    : UnaryOpNode(operand) {}

CosNode::~CosNode() {}

double CosNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::cos(opVal);
    Trace::addTransformation("Evaluating CosNode", toString(), std::to_string(result));
    return result;
}

std::string CosNode::toString() const {
    return "cos(" + operand->toString() + ")";
}

// **Simplification**
Node* CosNode::simplify() const {
    std::string before = toString();
    Node* simplified = new CosNode(operand->simplify());
    Trace::addTransformation("Simplify CosNode", before, simplified->toString());
    return simplified;
}

// **Differentiation (d/dx cos(x) = -sin(x) * dx)**
Node* CosNode::derivative(const std::string& variable) const {
    std::string before = toString();
    Node* derivativeResult = new MultiplicationNode(
        new NumberNode(-1), // Negative sign from differentiation
        new MultiplicationNode(new SinNode(operand->clone()), operand->derivative(variable))
    );
    Trace::addTransformation("Differentiate CosNode", before, derivativeResult->toString());
    return derivativeResult;
}

// **Substitution**
Node* CosNode::substitute(const std::string& variable, Node* value) const {
    std::string before = toString();
    Node* substituted = new CosNode(operand->substitute(variable, value));
    Trace::addTransformation("Substituting in CosNode", before, substituted->toString());
    return substituted;
}

// **Clone**
Node* CosNode::clone() const {
    return new CosNode(operand->clone());
}

} // namespace Expression
