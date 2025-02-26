#include "expression/multiplication_node.h"
#include "expression/number_node.h"
#include "expression/addition_node.h"

namespace Expression {

MultiplicationNode::MultiplicationNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

MultiplicationNode::~MultiplicationNode() {}

double MultiplicationNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal * rightVal;
    Trace::add("Evaluating MultiplicationNode: (" + left->toString() + " * " + right->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string MultiplicationNode::toString() const {
    return "(" + left->toString() + " * " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* MultiplicationNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();

    // Constant folding: If both operands are numbers, multiply them
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            return new NumberNode(leftNum->getValue() * rightNum->getValue());
        }
    }

    // Identity Rule: x * 1 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 1) return leftSimplified;
        if (rightNum->getValue() == 0) return new NumberNode(0);
    }
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 1) return rightSimplified;
        if (leftNum->getValue() == 0) return new NumberNode(0);
    }

    return new MultiplicationNode(leftSimplified, rightSimplified);
}

// **Symbolic Differentiation (Product Rule)**
Node* MultiplicationNode::derivative(const std::string& variable) const {
    Node* leftDerivative = left->derivative(variable);
    Node* rightDerivative = right->derivative(variable);

    Node* term1 = new MultiplicationNode(leftDerivative, right->clone());
    Node* term2 = new MultiplicationNode(left->clone(), rightDerivative);

    return new AdditionNode(term1, term2);
}

// **Symbolic Substitution**
Node* MultiplicationNode::substitute(const std::string& variable, Node* value) const {
    return new MultiplicationNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* MultiplicationNode::clone() const {
    return new MultiplicationNode(left->clone(), right->clone());
}

} // namespace Expression
