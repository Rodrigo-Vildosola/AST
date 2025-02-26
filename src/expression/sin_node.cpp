#include "expression/sin_node.h"
#include <cmath>
#include <sstream>
#include <string>

namespace Expression {

SinNode::SinNode(Node* operand)
    : UnaryOpNode(operand) {}

SinNode::~SinNode() {}

double SinNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::sin(opVal);
    Trace::add("Evaluating SinNode: sin(" + operand->toString() +
               ") = " + std::to_string(result));
    return result;
}

std::string SinNode::toString() const {
    return "sin(" + operand->toString() + ")";
}

} // namespace Expression
