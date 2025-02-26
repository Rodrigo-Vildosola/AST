#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include "node.h"
#include <string>

namespace Expression {

class NodeFactory;

// Variable nodes represent named values.
class VariableNode : public Node {
public:
    explicit VariableNode(const std::string& name);
    virtual ~VariableNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // Full Arena approach
    virtual Node* simplify(NodeFactory &factory) const override;
    virtual Node* derivative(const std::string& variable, NodeFactory &factory) const override;
    virtual Node* substitute(const std::string& variable, Node* value, NodeFactory &factory) const override;
    virtual Node* clone(NodeFactory &factory) const override;

    const std::string& getName() const { return name; }

    virtual bool extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const override;

private:
    std::string name;
};

} // namespace Expression

#endif
