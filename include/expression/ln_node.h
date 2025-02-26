#ifndef LNNODE_HPP
#define LNNODE_HPP

#include "unary_op_node.h"
#include "number_node.h"

namespace Expression {

// Represents natural logarithm: ln(x).
class LnNode : public UnaryOpNode {
public:
    explicit LnNode(Node* operand);
    virtual ~LnNode();

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
