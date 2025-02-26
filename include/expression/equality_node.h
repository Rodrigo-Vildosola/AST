#ifndef EQUALITY_NODE_H
#define EQUALITY_NODE_H

#include "node.h"

namespace Expression {

// EqualityNode represents an equation (e.g., A == B).
class EqualityNode : public Node {
public:
    EqualityNode(Node* left, Node* right);
    virtual ~EqualityNode();

    virtual double evaluate(const Env &env) override;
    virtual std::string toString() const override;

    // **New symbolic methods**
    virtual Node* simplify() const override;
    virtual Node* derivative(const std::string& variable) const override;
    virtual Node* substitute(const std::string& variable, Node* value) const override;
    virtual Node* clone() const override;
    
    // **Equation Solving**
    virtual Node* solveFor(const std::string& variable) const;

private:
    Node* left;
    Node* right;
};

} // namespace Expression

#endif
