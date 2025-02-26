#include "expression/equality_node.h"
#include "expression/addition_node.h"
#include "expression/subtraction_node.h"
#include "expression/multiplication_node.h"
#include "expression/variable_node.h"
#include "expression/number_node.h"

#include "tracing/trace.h"

namespace Expression {

// A naive helper that tries to extract coefficients from a simplified AST.
// It assumes the expression is linear in the variable `var`.
// If successful, it sets a and b such that the expression is a*x + b.
bool extractLinearCoeffs(Node* expr, const std::string &var, double &a, double &b) {
    // If the expression is a constant.
    if(auto num = dynamic_cast<NumberNode*>(expr)) {
        a = 0;
        b = num->getValue();
        return true;
    }
    // If the expression is the variable.
    if(auto varNode = dynamic_cast<VariableNode*>(expr)) {
        if(varNode->toString() == var) {
            a = 1;
            b = 0;
            return true;
        }
        return false; // some other variable
    }
    // If the expression is a multiplication.
    if(auto mult = dynamic_cast<MultiplicationNode*>(expr)) {
        // Try to find a constant multiplied by the variable.
        double c = 0;
        bool foundConstant = false;
        Node* other = nullptr;
        if(auto leftNum = dynamic_cast<NumberNode*>(mult->left)) {
            c = leftNum->getValue();
            foundConstant = true;
            other = mult->right;
        } else if(auto rightNum = dynamic_cast<NumberNode*>(mult->right)) {
            c = rightNum->getValue();
            foundConstant = true;
            other = mult->left;
        }
        if(foundConstant && other) {
            if(auto otherVar = dynamic_cast<VariableNode*>(other)) {
                if(otherVar->toString() == var) {
                    a = c;
                    b = 0;
                    return true;
                }
            }
        }
    }
    // If the expression is an addition.
    if(auto add = dynamic_cast<AdditionNode*>(expr)) {
        double a1 = 0, b1 = 0, a2 = 0, b2 = 0;
        bool ok1 = extractLinearCoeffs(add->left, var, a1, b1);
        bool ok2 = extractLinearCoeffs(add->right, var, a2, b2);
        a = a1 + a2;
        b = b1 + b2;
        return ok1 || ok2;
    }
    // If the expression is a subtraction.
    if(auto sub = dynamic_cast<SubtractionNode*>(expr)) {
        double a1 = 0, b1 = 0, a2 = 0, b2 = 0;
        bool ok1 = extractLinearCoeffs(sub->left, var, a1, b1);
        bool ok2 = extractLinearCoeffs(sub->right, var, a2, b2);
        a = a1 - a2;
        b = b1 - b2;
        return ok1 || ok2;
    }
    return false; // Cannot extract coefficients.
}


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
    // Form the equation f(x) = 0 by subtracting right from left.
    Node* diff = new SubtractionNode(left->clone(), right->clone());
    Node* simplifiedDiff = diff->simplify();
    delete diff;  // we no longer need the unsimplified difference

    double a = 0;
    double b = 0;
    if(!extractLinearCoeffs(simplifiedDiff, variable, a, b) || a == 0) {
        // Could not extract a linear form or a is 0.
        Trace::addTransformation("Solving equation", simplifiedDiff->toString(), "Unable to solve linearly");
        return simplifiedDiff->clone();  // or return nullptr, depending on design.
    }

    double solution = -b / a;
    Node* solNode = new NumberNode(solution);
    Trace::addTransformation("Solving equation", simplifiedDiff->toString(), solNode->toString());
    return solNode;
}

} // namespace Expression
