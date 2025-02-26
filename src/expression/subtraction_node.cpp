#include "expression/subtraction_node.h"
#include "expression/number_node.h"
#include "tracing/trace.h"

namespace Expression {

SubtractionNode::SubtractionNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

SubtractionNode::~SubtractionNode() {}

double SubtractionNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal - rightVal;
    Trace::addTransformation("Evaluating SubtractionNode", toString(), std::to_string(result));
    return result;
}

std::string SubtractionNode::toString() const {
    return "(" + left->toString() + " - " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* SubtractionNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();

    // If both sides are numbers, perform constant folding.
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            return new NumberNode(leftNum->getValue() - rightNum->getValue());
        }
    }

    // Identity rule: x - 0 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 0) {
            return leftSimplified;
        }
    }

    // x - x = 0
    if (left->toString() == right->toString()) {
        return new NumberNode(0);
    }

    return new SubtractionNode(leftSimplified, rightSimplified);
}

// **Symbolic Differentiation**
Node* SubtractionNode::derivative(const std::string& variable) const {
    return new SubtractionNode(left->derivative(variable), right->derivative(variable));
}

// **Symbolic Substitution**
Node* SubtractionNode::substitute(const std::string& variable, Node* value) const {
    return new SubtractionNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* SubtractionNode::clone() const {
    return new SubtractionNode(left->clone(), right->clone());
}

} // namespace Expression
