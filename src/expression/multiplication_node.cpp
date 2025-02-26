#include "expression/multiplication_node.h"
#include "expression/number_node.h"
#include "expression/addition_node.h"
#include "tracing/trace.h"

namespace Expression {

MultiplicationNode::MultiplicationNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

MultiplicationNode::~MultiplicationNode() {}

double MultiplicationNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal * rightVal;
    Trace::addTransformation("Evaluating MultiplicationNode", toString(), std::to_string(result));
    return result;
}

std::string MultiplicationNode::toString() const {
    return "(" + left->toString() + " * " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* MultiplicationNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();
    
    std::string before = toString();

    // If both sides are numbers, perform constant folding.
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            Node* simplified = new NumberNode(leftNum->getValue() * rightNum->getValue());
            Trace::addTransformation("Constant folding in MultiplicationNode", before, simplified->toString());
            return simplified;
        }
    }

    // Identity Rule: x * 1 = x, x * 0 = 0
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 1) {
            Trace::addTransformation("Simplify MultiplicationNode", before, leftSimplified->toString());
            return leftSimplified;
        }
        if (rightNum->getValue() == 0) {
            Trace::addTransformation("Simplify MultiplicationNode", before, "0");
            return new NumberNode(0);
        }
    }

    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 1) {
            Trace::addTransformation("Simplify MultiplicationNode", before, rightSimplified->toString());
            return rightSimplified;
        }
        if (leftNum->getValue() == 0) {
            Trace::addTransformation("Simplify MultiplicationNode", before, "0");
            return new NumberNode(0);
        }
    }

    Node* simplified = new MultiplicationNode(leftSimplified, rightSimplified);
    Trace::addTransformation("Simplify MultiplicationNode", before, simplified->toString());
    return simplified;
}

// **Symbolic Differentiation (Product Rule)**
Node* MultiplicationNode::derivative(const std::string& variable) const {
    std::string before = toString();
    Node* term1 = new MultiplicationNode(left->derivative(variable), right->clone());
    Node* term2 = new MultiplicationNode(left->clone(), right->derivative(variable));
    Node* result = new AdditionNode(term1, term2);
    Trace::addTransformation("Differentiate MultiplicationNode", before, result->toString());
    return result;
}

// **Symbolic Substitution**
Node* MultiplicationNode::substitute(const std::string& variable, Node* value) const {
    std::string before = toString();
    Node* substituted = new MultiplicationNode(left->substitute(variable, value), right->substitute(variable, value));
    Trace::addTransformation("Substituting in MultiplicationNode", before, substituted->toString());
    return substituted;
}

// **Clone**
Node* MultiplicationNode::clone() const {
    return new MultiplicationNode(left->clone(), right->clone());
}

} // namespace Expression
