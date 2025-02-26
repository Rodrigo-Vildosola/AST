#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H

#include "node.h"
#include <vector>
#include <functional>
#include <string>
#include <stdexcept>

namespace Expression {

// Function nodes now support an arbitrary number of arguments with an exact argument count.
class FunctionNode : public Node {
public:
    // The callback accepts a vector of double values (one for each argument)
    // and returns a double.
    using FunctionCallback = std::function<double(const std::vector<double>&)>;

    // The constructor takes the function name, the expected number of arguments,
    // a vector of pointers to argument nodes, and a callback to perform the function evaluation.
    FunctionNode(const std::string& name, int expectedArgCount, const std::vector<Node*>& arguments, FunctionCallback callback);
    virtual ~FunctionNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

private:
    std::string name;
    int expectedArgCount;
    std::vector<Node*> arguments;
    FunctionCallback callback;
};

} // namespace Expression

#endif
