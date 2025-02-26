#include "expression/ln_node.h"
#include "expression/division_node.h"
#include "tracing/trace.h"

namespace Expression {

LnNode::LnNode(Node* operand)
    : UnaryOpNode(operand) {}

LnNode::~LnNode() {}

double LnNode::evaluate(const Env &env) {
    double operandVal = operand->evaluate(env);

    if (operandVal <= 0) {
        throw std::runtime_error("Math error: ln of non-positive number.");
    }

    double result = std::log(operandVal);
    Trace::addTransformation("Evaluating LnNode", toString(), std::to_string(result));
    return result;
}

std::string LnNode::toString() const {
    return "ln(" + operand->toString() + ")";
}

// **Symbolic Simplification**
Node* LnNode::simplify() const {
    Node* simplifiedOperand = operand->simplify();

    // ln(1) = 0
    if (auto numNode = dynamic_cast<NumberNode*>(simplifiedOperand)) {
        if (numNode->getValue() == 1) {
            return new NumberNode(0);
        }
    }

    return new LnNode(simplifiedOperand);
}

// **Symbolic Differentiation**
Node* LnNode::derivative(const std::string& variable) const {
    // d/dx ln(x) = 1/x
    return new DivisionNode(new NumberNode(1), operand->clone());
}

// **Symbolic Substitution**
Node* LnNode::substitute(const std::string& variable, Node* value) const {
    return new LnNode(operand->substitute(variable, value));
}

// **Clone**
Node* LnNode::clone() const {
    return new LnNode(operand->clone());
}

} // namespace Expression
