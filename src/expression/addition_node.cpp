#include "expression/addition_node.h"


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

} // namespace Expression
