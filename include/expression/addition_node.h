// File: AdditionNode.h
#ifndef ADDITIONNODE_HPP
#define ADDITIONNODE_HPP

#include "binary_op_node.h"

namespace Expression {

class NodeFactory;

// Represents addition: left + right.
class AdditionNode : public BinaryOpNode {
public:
    AdditionNode(Node* left, Node* right);
    virtual ~AdditionNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **Full Arena approach**: pass NodeFactory to all these methods
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;
};

} // namespace Expression

#endif
