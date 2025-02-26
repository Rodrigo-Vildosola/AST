#ifndef MULTIPLICATIONNODE_HPP
#define MULTIPLICATIONNODE_HPP

#include "binary_op_node.h"

namespace Expression {

class NodeFactory;

// Represents multiplication: left * right.
class MultiplicationNode : public BinaryOpNode {
public:
    MultiplicationNode(Node* left, Node* right);
    virtual ~MultiplicationNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // Full Arena
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;
};

} // namespace Expression

#endif
