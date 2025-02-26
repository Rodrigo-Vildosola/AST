#ifndef ADDITIONNODE_HPP
#define ADDITIONNODE_HPP

#include "binary_op_node.h"

namespace Expression {

// Represents addition: left + right.
class AdditionNode : public BinaryOpNode {
public:
    AdditionNode(Node* left, Node* right);
    virtual ~AdditionNode();
    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;
};

} // namespace Expression

#endif
