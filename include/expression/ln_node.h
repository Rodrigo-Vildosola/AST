#ifndef LNNODE_HPP
#define LNNODE_HPP

#include "unary_op_node.h"

namespace Expression {

class NodeFactory;

// Represents natural logarithm: ln(x).
class LnNode : public UnaryOpNode {
public:
    explicit LnNode(Node* operand);
    virtual ~LnNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // Full Arena
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;

    virtual bool extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const override;

};

} // namespace Expression

#endif
