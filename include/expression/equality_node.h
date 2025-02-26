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
private:
    Node* left;
    Node* right;
};

} // namespace Expression

#endif
