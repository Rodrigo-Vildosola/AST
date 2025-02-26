#ifndef SINNODE_HPP
#define SINNODE_HPP

#include "unary_op_node.h"

namespace Expression {

// Represents the sine function: sin(operand).
class SinNode : public UnaryOpNode {
public:
    explicit SinNode(Node* operand);
    virtual ~SinNode();
    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;
};

} // namespace Expression

#endif
