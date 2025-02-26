#include "expression/exponentiation_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/addition_node.h"
#include "expression/division_node.h"
#include "expression/ln_node.h"
#include "helpers/node_factory.h"   // NodeFactory
#include "tracing/trace.h"
#include <cmath>

namespace Expression {

ExponentiationNode::ExponentiationNode(Node* base, Node* exponent)
    : BinaryOpNode(base, exponent) {}

ExponentiationNode::~ExponentiationNode() {
    // no child deletion
}

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
Node* ExponentiationNode::simplify(NodeFactory &factory) const {
    Node* baseSimplified = left->simplify(factory);
    Node* exponentSimplified = right->simplify(factory);

    // x^0 = 1
    if (auto exponentNum = dynamic_cast<NumberNode*>(exponentSimplified)) {
        if (exponentNum->getValue() == 0) {
            return factory.num(1);
        }
        if (exponentNum->getValue() == 1) {
            return baseSimplified;
        }
    }
    // 0^x = 0 (except 0^0)
    if (auto baseNum = dynamic_cast<NumberNode*>(baseSimplified)) {
        if (baseNum->getValue() == 0) {
            return factory.num(0);
        }
        if (baseNum->getValue() == 1) {
            return factory.num(1);
        }
    }
    return factory.exp(baseSimplified, exponentSimplified);
}

// **Symbolic Differentiation (General Power Rule)**
Node* ExponentiationNode::derivative(const std::string& variable, NodeFactory &factory) const {
    // If exponent is constant, apply power rule: d/dx (f(x)^n) = n * f(x)^(n-1) * f'(x)
    if (auto exponentNum = dynamic_cast<NumberNode*>(right)) {
        double n = exponentNum->getValue();
        Node* factor1 = factory.mul( factory.num(n),
            factory.exp(left->clone(factory), factory.num(n - 1))
        );
        Node* factor2 = left->derivative(variable, factory);
        return factory.mul(factor1, factor2);
    }

    // General case: d/dx (f^g) = f^g * ( g'(x)*ln(f) + g(x)*f'(x)/f )
    Node* term1 = factory.mul(right->derivative(variable, factory),
                              factory.ln(left->clone(factory)));
    Node* term2 = factory.mul(right->clone(factory),
                              factory.div(left->derivative(variable, factory),
                                          left->clone(factory)));
    Node* sum = factory.add(term1, term2);
    return factory.mul( factory.exp(left->clone(factory), right->clone(factory)), sum );
}

// **Symbolic Substitution**
Node* ExponentiationNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    Node* newBase = left->substitute(variable, value, factory);
    Node* newExp = right->substitute(variable, value, factory);
    return factory.exp(newBase, newExp);
}

// **Clone**
Node* ExponentiationNode::clone(NodeFactory &factory) const {
    return factory.exp(left->clone(factory), right->clone(factory));
}

} // namespace Expression
