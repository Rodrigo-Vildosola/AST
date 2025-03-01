#pragma once

#include "helpers/node_factory.h"
#include "helpers/expr_operators.h"
#include "expression/node.h"
#include "solver/solver.h"
#include <string>
#include <sstream>

namespace Expression {

class Expr {
public:
    // Underlying node and a reference to the factory.
    Node* node;
    NodeFactory& factory;

    // Constructor.
    Expr(Node* n, NodeFactory &f) : node(n), factory(f) {}

    Expr(const Expr &other) : node(other.node), factory(other.factory) {}

    // Overload arithmetic operators.
    Expr operator+(const Expr &other) const {
        return Expr(factory.add(this->node, other.node), factory);
    }

    Expr operator-(const Expr &other) const {
        return Expr(factory.sub(this->node, other.node), factory);
    }

    Expr operator*(const Expr &other) const {
        return Expr(factory.mul(this->node, other.node), factory);
    }

    Expr operator/(const Expr &other) const {
        return Expr(factory.div(this->node, other.node), factory);
    }

    // For exponentiation, we'll overload the caret operator (^)
    Expr operator^(const Expr &other) const {
        return Expr(factory.exp(this->node, other.node), factory);
    }

    // Overload equality to create an equality node.
    // (Be careful: operator== is normally used for comparisons, so this is a domain-specific choice.)
    Expr operator==(const Expr &other) const {
        return Expr(factory.eq(this->node, other.node), factory);
    }

    // Helper: Convert to string.
    std::string toString() const {
        return node->toString();
    }

    // Evaluate the expression given an environment.
    double evaluate(const Env &env) const {
        return node->evaluate(env);
    }

    // Static helpers to create numbers and variables.
    static Expr Num(double value, NodeFactory &factory) {
        return Expr(factory.num(value), factory);
    }

    static Expr Var(const std::string &name, NodeFactory &factory) {
        return Expr(factory.var(name), factory);
    }

    // --- NEW FUNCTION: Solve for a variable ---
    // This function assumes that the current expression is an equation (i.e., an EqualityNode).
    // It returns a vector of real roots for the specified variable.
    std::vector<double> solveFor(const std::string &variable) const {
        const EqualityNode* eq = dynamic_cast<const EqualityNode*>(node);
        if (!eq) {
            throw std::runtime_error("Expression is not an equation.");
        }
        return ExtendedSolver::solve_equation(eq, variable, factory);
    }
};

// #define BUILD_EXPR(f, X) build_expr(f, [&](){ return (X); })

// inline Expr build_expr(NodeFactory &fact, std::function<Expr()> fn) {
//     // Just call fn. The user can still do local logic if needed.
//     return fn();
// }

inline Expr equals(const Expr &lhs, const Expr &rhs) {
    NodeFactory &f = lhs.factory; 
    // We assume both expressions share the same factory, which is typical
    // if you built them in the same scope. 
    return Expr(f.eq(lhs.node, rhs.node), f);
}


} // namespace Expression
