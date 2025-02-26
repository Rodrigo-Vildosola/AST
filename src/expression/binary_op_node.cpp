#include "expression/binary_op_node.h"
#include "memory/allocation_config.h"

namespace Expression {

BinaryOpNode::BinaryOpNode(Node* left, Node* right)
    : left(left), right(right) {}

BinaryOpNode::~BinaryOpNode() {
    if (AllocationConfig::getPolicy() == DeletionPolicy::Default) {
        delete left;
        delete right;
    }
}

} // namespace Expression
