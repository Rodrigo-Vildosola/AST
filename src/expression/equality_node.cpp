#include "expression/equality_node.h"
#include "expression/trace.h"
#include <sstream>
#include <cmath>
#include <string>

namespace Expression {

EqualityNode::EqualityNode(Node* left, Node* right)
    : left(left), right(right) {}

EqualityNode::~EqualityNode() {
    delete left;
    delete right;
}

double EqualityNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    bool equal = std::fabs(leftVal - rightVal) < 1e-9; // Using a small tolerance.
    Trace::add("Evaluating EqualityNode: (" + left->toString() + " == " + right->toString() +
               ") -> " + (equal ? "true" : "false"));
    return equal ? 1.0 : 0.0;
}

std::string EqualityNode::toString() const {
    return "(" + left->toString() + " == " + right->toString() + ")";
}

} // namespace Expression
