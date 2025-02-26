#ifndef NUMBERNODE_HPP
#define NUMBERNODE_HPP

#include "node.h"

namespace Expression {

class NodeFactory;

// Represents a numeric literal.
class NumberNode : public Node {
public:
    explicit NumberNode(double value);
    virtual ~NumberNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // Full Arena approach
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;

    double getValue() const;

private:
    double value;
};

} // namespace Expression

#endif
