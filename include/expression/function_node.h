#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H

#include "node.h"

namespace Expression {

// Function nodes now support an arbitrary number of arguments with an exact argument count.
class FunctionNode : public Node {
public:
    using FunctionCallback = std::function<double(const std::vector<double>&)>;

    FunctionNode(const std::string& name, int expectedArgCount, const std::vector<Node*>& arguments, FunctionCallback callback);
    virtual ~FunctionNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **New symbolic methods**
    virtual Node* simplify() const override;
    virtual Node* derivative(const std::string& variable) const override;
    virtual Node* substitute(const std::string& variable, Node* value) const override;
    virtual Node* clone() const override;

private:
    std::string name;
    int expectedArgCount;
    std::vector<Node*> arguments;
    FunctionCallback callback;
};

} // namespace Expression

#endif
