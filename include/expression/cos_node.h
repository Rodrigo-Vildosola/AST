#ifndef COSNODE_HPP
#define COSNODE_HPP

#include "unary_op_node.h"

namespace Expression {

// Represents sin(x).
class CosNode : public UnaryOpNode {
public:
    explicit CosNode(Node* operand);
    virtual ~CosNode();

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
