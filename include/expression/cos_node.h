#ifndef COSNODE_HPP
#define COSNODE_HPP

#include "unary_op_node.h"

namespace Expression {

class NodeFactory;

// Represents sin(x).
class CosNode : public UnaryOpNode {
public:
    explicit CosNode(Node* operand);
    virtual ~CosNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **New symbolic methods**
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string &variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string &variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;

    virtual bool extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const override;

};

} // namespace Expression

#endif
