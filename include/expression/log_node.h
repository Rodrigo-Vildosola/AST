#ifndef LOGNODE_HPP
#define LOGNODE_HPP

#include "binary_op_node.h"
#include "number_node.h"

namespace Expression {

// Represents logarithm: log_base(x).
class LogNode : public BinaryOpNode {
public:
    LogNode(Node* base, Node* operand);
    virtual ~LogNode();

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
