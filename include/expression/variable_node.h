#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include "node.h"

namespace Expression {

// Define the environment type to map variable names to their values.
using Env = std::unordered_map<std::string, double>;

// Variable nodes represent named values.
// They inherit directly from Node.
class VariableNode : public Node {
public:
    explicit VariableNode(const std::string& name);
    virtual ~VariableNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;
private:
    std::string name;
};

// Global environment instance used for variable lookup.
extern Env globalEnv;

} // namespace Expression

#endif
