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

    // **New symbolic methods**
    virtual Node* simplify() const override;
    virtual Node* derivative(const std::string& variable) const override;
    virtual Node* substitute(const std::string& variable, Node* value) const override;
    virtual Node* clone() const override;

    double getValue() const;
private:
    double value;
};

} // namespace Expression

#endif
