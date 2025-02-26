#include "expression/binary_op_node.h"

namespace Expression {

BinaryOpNode::BinaryOpNode(Node* left, Node* right)
    : left(left), right(right) {}

BinaryOpNode::~BinaryOpNode() {
    delete left;
    delete right;
}

} // namespace Expression
