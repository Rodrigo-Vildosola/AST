#ifndef NODE_HPP
#define NODE_HPP

#include "_pch.h"
#include "tracing/trace.h"

namespace Expression {

using Env = std::unordered_map<std::string, double>;
using FunctionCallback = std::function<double(const std::vector<double>&)>;

class NodeFactory;

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

    /**
     * @brief Attempt to extract coefficients from the expression assuming it is linear in `var`.
     *
     * If the node represents an expression of the form a*x + b,
     * then it sets coeff = a and constant = b and returns true.
     * Otherwise, returns false.
     */
    virtual bool extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const {
        return false;
    }
};

} // namespace Expression

#endif
