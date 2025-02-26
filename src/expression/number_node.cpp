#include "expression/number_node.h"
#include <sstream>

namespace Expression {

NumberNode::NumberNode(double value) : value(value) {}

NumberNode::~NumberNode() {}

double NumberNode::evaluate(const Env &env) {
    Trace::add("Evaluating NumberNode: " + toString());
    return value;
}

std::string NumberNode::toString() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

} // namespace Expression
