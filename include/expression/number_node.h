#ifndef NUMBERNODE_HPP
#define NUMBERNODE_HPP

#include "node.h"

namespace Expression {

// Represents a numeric literal.
class NumberNode : public Node {
public:
    explicit NumberNode(double value);
    virtual ~NumberNode();
    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;
private:
    double value;
};

} // namespace Expression

#endif
