#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include "node.h"

namespace Expression {

// Variable nodes represent named values.
class VariableNode : public Node {
public:
    explicit VariableNode(const std::string& name);
    virtual ~VariableNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **New symbolic methods**
    virtual Node* simplify() const override;
    virtual Node* derivative(const std::string& variable) const override;
    virtual Node* substitute(const std::string& variable, Node* value) const override;
    virtual Node* clone() const override;

private:
    std::string name;
};

} // namespace Expression

#endif
