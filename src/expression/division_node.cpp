#include "expression/division_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/subtraction_node.h"
#include "expression/exponentiation_node.h"
#include "tracing/trace.h"

namespace Expression {

DivisionNode::DivisionNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

DivisionNode::~DivisionNode() {}

double DivisionNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);

    if (rightVal == 0) {
        throw std::runtime_error("Division by zero error in " + toString());
    }

    double result = leftVal / rightVal;
    Trace::addTransformation("Evaluating DivisionNode", toString(), std::to_string(result));
    return result;
}

std::string DivisionNode::toString() const {
    return "(" + left->toString() + " / " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* DivisionNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();

    // If both sides are numbers, perform constant folding.
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            if (rightNum->getValue() == 0) {
                throw std::runtime_error("Attempted division by zero in simplification.");
            }
            return new NumberNode(leftNum->getValue() / rightNum->getValue());
        }
    }

    // Identity rule: x / 1 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 1) {
            return leftSimplified;
        }
    }

    // Zero rule: 0 / x = 0
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 0) {
            return new NumberNode(0);
        }
    }

    return new DivisionNode(leftSimplified, rightSimplified);
}

// **Symbolic Differentiation (Quotient Rule)**
Node* DivisionNode::derivative(const std::string& variable) const {
    Node* f_prime = left->derivative(variable);
    Node* g_prime = right->derivative(variable);

    Node* numerator = new SubtractionNode(
        new MultiplicationNode(f_prime, right->clone()),
        new MultiplicationNode(left->clone(), g_prime)
    );

    Node* denominator = new MultiplicationNode(right->clone(), right->clone());

    return new DivisionNode(numerator, denominator);
}

// **Symbolic Substitution**
Node* DivisionNode::substitute(const std::string& variable, Node* value) const {
    return new DivisionNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* DivisionNode::clone() const {
    return new DivisionNode(left->clone(), right->clone());
}

} // namespace Expression
