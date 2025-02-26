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
    Trace::add("Evaluating CosNode: cos(" + operand->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string CosNode::toString() const {
    return "cos(" + operand->toString() + ")";
}

// **Simplification**
Node* CosNode::simplify() const {
    return new CosNode(operand->simplify());
}

// **Differentiation (d/dx cos(x) = -sin(x) * dx)**
Node* CosNode::derivative(const std::string& variable) const {
    // Apply chain rule: d/dx cos(f(x)) = -sin(f(x)) * f'(x)
    return new MultiplicationNode(
        new NumberNode(-1), // Negative sign from differentiation
        new MultiplicationNode(new SinNode(operand->clone()), operand->derivative(variable))
    );
}

// **Substitution**
Node* CosNode::substitute(const std::string& variable, Node* value) const {
    return new CosNode(operand->substitute(variable, value));
}

// **Clone**
Node* CosNode::clone() const {
    return new CosNode(operand->clone());
}

} // namespace Expression
