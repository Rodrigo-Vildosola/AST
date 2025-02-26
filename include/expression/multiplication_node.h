#ifndef MULTIPLICATIONNODE_HPP
#define MULTIPLICATIONNODE_HPP

#include "binary_op_node.h"

namespace Expression {

// Represents addition: left + right.
class MultiplicationNode : public BinaryOpNode {
public:
    MultiplicationNode(Node* left, Node* right);
    virtual ~MultiplicationNode();
    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;
};

} // namespace Expression

#endif
