#include "expression/addition_node.h"
#include "expression/number_node.h"


namespace Expression {

AdditionNode::AdditionNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

AdditionNode::~AdditionNode() {}

double AdditionNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal + rightVal;
    Trace::add("Evaluating AdditionNode: (" + left->toString() + " + " + right->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string AdditionNode::toString() const {
    return "(" + left->toString() + " + " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* AdditionNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();

    // If both sides are numbers, perform constant folding.
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            return new NumberNode(leftNum->getValue() + rightNum->getValue());
        }
    }

    // Identity rule: x + 0 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 0) {
            return leftSimplified;
        }
    }

    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 0) {
            return rightSimplified;
        }
    }

    return new AdditionNode(leftSimplified, rightSimplified);
}

// **Symbolic Differentiation**
Node* AdditionNode::derivative(const std::string& variable) const {
    return new AdditionNode(left->derivative(variable), right->derivative(variable));
}

// **Symbolic Substitution**
Node* AdditionNode::substitute(const std::string& variable, Node* value) const {
    return new AdditionNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* AdditionNode::clone() const {
    return new AdditionNode(left->clone(), right->clone());
}

} // namespace Expression
