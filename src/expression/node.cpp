#include "expression/node.h"

namespace Expression {

std::ostringstream Trace::oss;

void Trace::add(const std::string &message) {
    oss << message << "\n";
}

void Trace::clear() {
    oss.str("");
    oss.clear();
}

std::string Trace::getTrace() {
    return oss.str();
}

Node::~Node() {}

} // namespace Expression
