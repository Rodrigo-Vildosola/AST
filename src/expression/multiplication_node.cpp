#include "expression/multiplication_node.h"
#include "expression/number_node.h"
#include "expression/addition_node.h"
#include "helpers/node_factory.h"
#include "tracing/trace.h"
#include <stdexcept>

namespace Expression {

MultiplicationNode::MultiplicationNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

MultiplicationNode::~MultiplicationNode() {
    // No child deletes in Full Arena
}

double MultiplicationNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal * rightVal;
    Trace::addTransformation("Evaluating MultiplicationNode", toString(), std::to_string(result));
    return result;
}

std::string MultiplicationNode::toString() const {
    return "(" + left->toString() + " * " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* MultiplicationNode::simplify(NodeFactory &factory) const {
    Node* leftSimplified = left->simplify(factory);
    Node* rightSimplified = right->simplify(factory);

    std::string before = toString();

    // If both sides are numbers, do constant folding
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            double val = leftNum->getValue() * rightNum->getValue();
            Node* simplified = factory.num(val);
            Trace::addTransformation("Constant folding in MultiplicationNode", before, simplified->toString());
            return simplified;
        }
    }

    // Identity Rule: x * 1 = x, x * 0 = 0
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 1) {
            Trace::addTransformation("Simplify MultiplicationNode", before, leftSimplified->toString());
            return leftSimplified;
        }
        if (rightNum->getValue() == 0) {
            Trace::addTransformation("Simplify MultiplicationNode", before, "0");
            return factory.num(0);
        }
    }
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (leftNum->getValue() == 1) {
            Trace::addTransformation("Simplify MultiplicationNode", before, rightSimplified->toString());
            return rightSimplified;
        }
        if (leftNum->getValue() == 0) {
            Trace::addTransformation("Simplify MultiplicationNode", before, "0");
            return factory.num(0);
        }
    }

    Node* simplified = factory.mul(leftSimplified, rightSimplified);
    Trace::addTransformation("Simplify MultiplicationNode", before, simplified->toString());
    return simplified;
}

// **Symbolic Differentiation (Product Rule)**
Node* MultiplicationNode::derivative(const std::string& variable, NodeFactory &factory) const {
    std::string before = toString();
    Node* dLeft = left->derivative(variable, factory);
    Node* dRight = right->derivative(variable, factory);

    // product rule: f'(x)g(x) + f(x)g'(x)
    Node* term1 = factory.mul(dLeft, right->clone(factory));
    Node* term2 = factory.mul(left->clone(factory), dRight);
    Node* result = factory.add(term1, term2);

    Trace::addTransformation("Differentiate MultiplicationNode", before, result->toString());
    return result;
}

// **Symbolic Substitution**
Node* MultiplicationNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    std::string before = toString();
    Node* newLeft = left->substitute(variable, value, factory);
    Node* newRight = right->substitute(variable, value, factory);
    Node* substituted = factory.mul(newLeft, newRight);
    Trace::addTransformation("Substituting in MultiplicationNode", before, substituted->toString());
    return substituted;
}

// **Clone**
Node* MultiplicationNode::clone(NodeFactory &factory) const {
    return factory.mul(left->clone(factory), right->clone(factory));
}

} // namespace Expression
