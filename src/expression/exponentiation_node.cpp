#include "expression/exponentiation_node.h"
#include "expression/multiplication_node.h"
#include "expression/division_node.h"
#include "expression/addition_node.h"
#include "expression/ln_node.h"
#include "tracing/trace.h"

namespace Expression {

ExponentiationNode::ExponentiationNode(Node* base, Node* exponent)
    : BinaryOpNode(base, exponent) {}

ExponentiationNode::~ExponentiationNode() {}

double ExponentiationNode::evaluate(const Env &env) {
    double baseVal = left->evaluate(env);
    double exponentVal = right->evaluate(env);
    
    if (baseVal == 0 && exponentVal <= 0) {
        throw std::runtime_error("Math error: 0 raised to a non-positive exponent.");
    }

    double result = std::pow(baseVal, exponentVal);
    Trace::addTransformation("Evaluating ExponentiationNode", toString(), std::to_string(result));
    return result;
}

std::string ExponentiationNode::toString() const {
    return "(" + left->toString() + " ^ " + right->toString() + ")";
}

// **Symbolic Simplification**
Node* ExponentiationNode::simplify() const {
    Node* baseSimplified = left->simplify();
    Node* exponentSimplified = right->simplify();

    // x^0 = 1
    if (auto exponentNum = dynamic_cast<NumberNode*>(exponentSimplified)) {
        if (exponentNum->getValue() == 0) {
            return new NumberNode(1);
        }
        if (exponentNum->getValue() == 1) {
            return baseSimplified;
        }
    }

    // 0^x = 0 (except when x = 0)
    if (auto baseNum = dynamic_cast<NumberNode*>(baseSimplified)) {
        if (baseNum->getValue() == 0) {
            return new NumberNode(0);
        }
        if (baseNum->getValue() == 1) {
            return new NumberNode(1);
        }
    }

    return new ExponentiationNode(baseSimplified, exponentSimplified);
}

// **Symbolic Differentiation (General Power Rule)**
Node* ExponentiationNode::derivative(const std::string& variable) const {
    // If exponent is constant, apply power rule: d/dx (f(x)^n) = n * f(x)^(n-1) * f'(x)
    if (auto exponentNum = dynamic_cast<NumberNode*>(right)) {
        double n = exponentNum->getValue();
        return new MultiplicationNode(
            new MultiplicationNode(new NumberNode(n), 
                new ExponentiationNode(left->clone(), new NumberNode(n - 1))
            ),
            left->derivative(variable)
        );
    }

    // General case: d/dx (f(x)^g(x)) = f^g * (g' * ln(f) + g * f'/f)
    Node* term1 = new MultiplicationNode(right->derivative(variable), new LnNode(left->clone()));
    Node* term2 = new MultiplicationNode(right->clone(), new DivisionNode(left->derivative(variable), left->clone()));

    Node* fullDerivative = new MultiplicationNode(new ExponentiationNode(left->clone(), right->clone()), 
                        new AdditionNode(term1, term2));

    return fullDerivative;
}

// **Symbolic Substitution**
Node* ExponentiationNode::substitute(const std::string& variable, Node* value) const {
    return new ExponentiationNode(left->substitute(variable, value), right->substitute(variable, value));
}

// **Clone**
Node* ExponentiationNode::clone() const {
    return new ExponentiationNode(left->clone(), right->clone());
}

} // namespace Expression
