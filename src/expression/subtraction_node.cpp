#include "expression/subtraction_node.h"
#include "expression/number_node.h"
#include "helpers/node_factory.h"
#include "tracing/trace.h"

namespace Expression {

SubtractionNode::SubtractionNode(Node* left, Node* right)
    : BinaryOpNode(left, right) {}

SubtractionNode::~SubtractionNode() {
    // no child deletion
}

double SubtractionNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    double result = leftVal - rightVal;
    Trace::addTransformation("Evaluating SubtractionNode", toString(), std::to_string(result));
    return result;
}

std::string SubtractionNode::toString() const {
    return "(" + left->toString() + " - " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* SubtractionNode::simplify(NodeFactory &factory) const {
    Node* leftSimplified = left->simplify(factory);
    Node* rightSimplified = right->simplify(factory);

    // If both sides are numbers => constant folding
    if (auto leftNum = dynamic_cast<NumberNode*>(leftSimplified)) {
        if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
            return factory.num(leftNum->getValue() - rightNum->getValue());
        }
    }

    // Identity rule: x - 0 = x
    if (auto rightNum = dynamic_cast<NumberNode*>(rightSimplified)) {
        if (rightNum->getValue() == 0) {
            return leftSimplified;
        }
    }

    // x - x = 0
    if (left->toString() == right->toString()) {
        return factory.num(0);
    }

    return factory.sub(leftSimplified, rightSimplified);
}

// **Symbolic Differentiation**
Node* SubtractionNode::derivative(const std::string& variable, NodeFactory &factory) const {
    Node* dLeft = left->derivative(variable, factory);
    Node* dRight = right->derivative(variable, factory);
    return factory.sub(dLeft, dRight);
}

// **Substitution**
Node* SubtractionNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    Node* newLeft = left->substitute(variable, value, factory);
    Node* newRight = right->substitute(variable, value, factory);
    return factory.sub(newLeft, newRight);
}

// **Clone**
Node* SubtractionNode::clone(NodeFactory &factory) const {
    return factory.sub(left->clone(factory), right->clone(factory));
}

} // namespace Expression
