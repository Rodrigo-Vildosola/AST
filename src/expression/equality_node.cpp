#include "expression/equality_node.h"
#include "expression/variable_node.h"
#include "expression/number_node.h"
#include "tracing/trace.h"

namespace Expression {

EqualityNode::EqualityNode(Node* left, Node* right)
    : left(left), right(right) {}

EqualityNode::~EqualityNode() {
    // delete left;
    // delete right;
}

double EqualityNode::evaluate(const Env &env) {
    double leftVal = left->evaluate(env);
    double rightVal = right->evaluate(env);
    bool equal = std::fabs(leftVal - rightVal) < 1e-9; // Small tolerance
    Trace::addTransformation("Evaluating EqualityNode", left->toString() + " == " + right->toString(), equal ? "true" : "false");
    return equal ? 1.0 : 0.0;
}

std::string EqualityNode::toString() const {
    return "(" + left->toString() + " == " + right->toString() + ")";
}

// **Simplify: Remove unnecessary expressions**
Node* EqualityNode::simplify() const {
    Node* leftSimplified = left->simplify();
    Node* rightSimplified = right->simplify();
    
    if (leftSimplified->toString() == rightSimplified->toString()) {
        Trace::addTransformation("Simplify EqualityNode", toString(), "true");
        return new NumberNode(1);
    }
    return new EqualityNode(leftSimplified, rightSimplified);
}

// **Derivative: The derivative of an equation is just the difference**
Node* EqualityNode::derivative(const std::string& variable) const {
    return new EqualityNode(left->derivative(variable), right->derivative(variable));
}

// **Substitute variable with a value/expression**
Node* EqualityNode::substitute(const std::string& variable, Node* value) const {
    return new EqualityNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone the equality node**
Node* EqualityNode::clone() const {
    return new EqualityNode(left->clone(), right->clone());
}

// **Solve for a given variable (simple rearrangement)**
Node* EqualityNode::solveFor(const std::string& variable) const {
    if (auto varNode = dynamic_cast<VariableNode*>(left)) {
        if (varNode->toString() == variable) {
            Trace::addTransformation("Solving equation", toString(), right->toString());
            return right->clone();
        }
    } else if (auto varNode = dynamic_cast<VariableNode*>(right)) {
        if (varNode->toString() == variable) {
            Trace::addTransformation("Solving equation", toString(), left->toString());
            return left->clone();
        }
    }

    Trace::addTransformation("Unable to solve equation for " + variable, toString(), "Unsolved");
    return clone(); // Return as-is if unsolvable
}

} // namespace Expression
