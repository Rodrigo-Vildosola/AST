#ifndef BINARYOPNODE_HPP
#define BINARYOPNODE_HPP

#include "node.h"

namespace Expression {

// Base class for binary operations (e.g., addition, multiplication).
class BinaryOpNode : public Node {
public:
    BinaryOpNode(Node* left, Node* right);
    virtual ~BinaryOpNode();

    Node* left;
    Node* right;
protected:
};

} // namespace Expression

#endif
