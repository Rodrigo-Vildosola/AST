#ifndef UNARYOPNODE_HPP
#define UNARYOPNODE_HPP

#include "node.h"

namespace Expression {

// Base class for unary operations (e.g., sin, cos, negation).
class UnaryOpNode : public Node {
public:
    explicit UnaryOpNode(Node* operand);
    virtual ~UnaryOpNode();
protected:
    Node* operand;
};

} // namespace Expression

#endif
