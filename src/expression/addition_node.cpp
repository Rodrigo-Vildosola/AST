#include "expression/addition_node.h"
#include "expression/number_node.h"
#include "tracing/trace.h"

namespace Expression {

AdditionNode::AdditionNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

AdditionNode::~AdditionNode() {}

double AdditionNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal + rightVal;
    Trace::addTransformation("Evaluating AdditionNode", toString(), std::to_string(result));
    return result;
}

std::string AdditionNode::toString() const {
    return "(" + left->toString() + " + " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* AdditionNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();

    std::string before = toString();

    // If both sides are numbers, perform constant folding.
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            Node* simplified = new NumberNode(leftNum->getValue() + rightNum->getValue());
            Trace::addTransformation("Constant folding in AdditionNode", before, simplified->toString());
            return simplified;
        }
    }

    // Identity rule: x + 0 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 0) {
            Trace::addTransformation("Simplify AdditionNode", before, leftSimplified->toString());
            return leftSimplified;
        }
    }

    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 0) {
            Trace::addTransformation("Simplify AdditionNode", before, rightSimplified->toString());
            return rightSimplified;
        }
    }

    Node* simplified = new AdditionNode(leftSimplified, rightSimplified);
    Trace::addTransformation("Simplify AdditionNode", before, simplified->toString());
    return simplified;
}

// **Symbolic Differentiation**
Node* AdditionNode::derivative(const std::string& variable) const {
    std::string before = toString();
    Node* derivativeResult = new AdditionNode(left->derivative(variable), right->derivative(variable));
    Trace::addTransformation("Differentiate AdditionNode", before, derivativeResult->toString());
    return derivativeResult;
}

// **Symbolic Substitution**
Node* AdditionNode::substitute(const std::string& variable, Node* value) const {
    std::string before = toString();
    Node* substituted = new AdditionNode(left->substitute(variable, value), right->substitute(variable, value));
    Trace::addTransformation("Substituting in AdditionNode", before, substituted->toString());
    return substituted;
}

// **Clone**
Node* AdditionNode::clone() const {
    return new AdditionNode(left->clone(), right->clone());
}

} // namespace Expression
