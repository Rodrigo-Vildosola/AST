#ifndef NODE_HPP
#define NODE_HPP

#include "_pch.h"
#include "tracing/trace.h"

namespace Expression {

using Env = std::unordered_map<std::string, double>;

// Abstract base class for all expression nodes.
class Node {
public:
    virtual ~Node();
    // Evaluate the expression represented by this node.
    virtual double evaluate(const Env &env) = 0;
    // Return a string representation of the node.
    virtual std::string toString() const = 0;

    // **NEW METHODS FOR SYMBOLIC COMPUTATION**
    virtual Node* simplify() const = 0;  // Simplify the expression if possible.
    virtual Node* derivative(const std::string& variable) const = 0;  // Compute derivative w.r.t a variable.
    virtual Node* substitute(const std::string& variable, Node* value) const = 0;  // Substitute a variable with an expression.
    virtual Node* clone() const = 0;  // Deep copy of this node.
};

} // namespace Expression

#endif
