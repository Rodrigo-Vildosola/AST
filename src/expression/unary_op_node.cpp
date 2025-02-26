#include "expression/unary_op_node.h"

namespace Expression {

UnaryOpNode::UnaryOpNode(Node* operand) : operand(operand) {}

UnaryOpNode::~UnaryOpNode() {
    // delete operand;
}

} // namespace Expression
