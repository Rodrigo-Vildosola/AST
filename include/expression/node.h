#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <sstream>
#include <unordered_map>
#include "trace.h"

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
};

} // namespace Expression

#endif
