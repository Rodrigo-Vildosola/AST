#ifndef SINNODE_HPP
#define SINNODE_HPP

#include "unary_op_node.h"

namespace Expression {

// Represents sin(x).
class SinNode : public UnaryOpNode {
public:
    explicit SinNode(Node* operand);
    virtual ~SinNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **New symbolic methods**
    virtual Node* simplify() const override;
    virtual Node* derivative(const std::string& variable) const override;
    virtual Node* substitute(const std::string& variable, Node* value) const override;
    virtual Node* clone() const override;
};

} // namespace Expression

#endif
