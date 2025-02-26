#include "expression/multiplication_node.h"


namespace Expression {

MultiplicationNode::MultiplicationNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

MultiplicationNode::~MultiplicationNode() {}

double MultiplicationNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal * rightVal;
    Trace::add("Evaluating MultiplicationNode: (" + left->toString() + " + " + right->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string MultiplicationNode::toString() const {
    return "(" + left->toString() + " * " + right->toString() + ")";
}

} // namespace Expression
