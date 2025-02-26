#ifndef NODE_HPP
#define NODE_HPP

#include "_pch.h"
#include "tracing/trace.h"
#include "helpers/node_factory.h"

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
    virtual Node* simplify(NodeFactory &e) const = 0;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const = 0;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const = 0;
    virtual Node* clone(NodeFactory &factory) const = 0;
};

} // namespace Expression

#endif
