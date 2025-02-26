#include "expression/log_node.h"
#include "expression/ln_node.h"
#include "expression/division_node.h"
#include "expression/multiplication_node.h"
#include "tracing/trace.h"

namespace Expression {

LogNode::LogNode(Node* base, Node* operand)
    : BinaryOpNode(base, operand) {}

LogNode::~LogNode() {}

double LogNode::evaluate(const Env &env) {
    double baseVal = left->evaluate(env);
    double operandVal = right->evaluate(env);

    if (baseVal <= 0 || baseVal == 1 || operandVal <= 0) {
        throw std::runtime_error("Math error: log with invalid base or operand.");
    }

    double result = std::log(operandVal) / std::log(baseVal);
    Trace::addTransformation("Evaluating LogNode", toString(), std::to_string(result));
    return result;
}

std::string LogNode::toString() const {
    return "log(" + left->toString() + ", " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* LogNode::simplify() const {
    Node* baseSimplified = left->simplify();
    Node* operandSimplified = right->simplify();

    // log_b(b) = 1
    if (baseSimplified->toString() == operandSimplified->toString()) {
        return new NumberNode(1);
    }

    return new LogNode(baseSimplified, operandSimplified);
}

// **Symbolic Differentiation**
Node* LogNode::derivative(const std::string& variable) const {
    // d/dx log_b(x) = 1 / (x ln(b))
    return new DivisionNode(
        new NumberNode(1),
        new MultiplicationNode(right->clone(), new LnNode(left->clone()))
    );
}

// **Symbolic Substitution**
Node* LogNode::substitute(const std::string& variable, Node* value) const {
    return new LogNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* LogNode::clone() const {
    return new LogNode(left->clone(), right->clone());
}

} // namespace Expression
