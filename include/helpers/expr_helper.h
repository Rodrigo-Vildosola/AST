#ifndef EXPR_HELPER_H
#define EXPR_HELPER_H

#include "expression/number_node.h"
#include "expression/addition_node.h"
#include "expression/subtraction_node.h"
#include "expression/multiplication_node.h"
#include "expression/division_node.h"
#include "expression/exponentiation_node.h"
#include "expression/sin_node.h"
#include "expression/cos_node.h"
#include "expression/ln_node.h"
#include "expression/log_node.h"
#include "expression/variable_node.h"
#include "expression/equality_node.h"
#include "expression/function_node.h"
#include "memory/expr_arena.h"

namespace Expression {

class ExprHelper {
private:
    ExprArena& arena;  // Reference to memory arena

public:
    explicit ExprHelper(ExprArena& a) : arena(a) {}

    // Number & Variable Nodes
    Node* num(double value) { return arena.make<NumberNode>(value); }
    Node* var(const std::string &name) { return arena.make<VariableNode>(name); }

    // Binary Operations
    Node* add(Node* left, Node* right) { return arena.make<AdditionNode>(left, right); }
    Node* sub(Node* left, Node* right) { return arena.make<SubtractionNode>(left, right); }
    Node* mul(Node* left, Node* right) { return arena.make<MultiplicationNode>(left, right); }
    Node* div(Node* left, Node* right) { return arena.make<DivisionNode>(left, right); }
    Node* exp(Node* base, Node* exponent) { return arena.make<ExponentiationNode>(base, exponent); }

    // Unary Operations
    Node* sin(Node* operand) { return arena.make<SinNode>(operand); }
    Node* cos(Node* operand) { return arena.make<CosNode>(operand); }
    Node* ln(Node* operand) { return arena.make<LnNode>(operand); }
    Node* log(Node* base, Node* operand) { return arena.make<LogNode>(base, operand); }

    // Equality & Functions
    Node* eq(Node* left, Node* right) { return arena.make<EqualityNode>(left, right); }
    Node* func(const std::string &name, int expectedArgCount, const std::vector<Node*>& args,
               FunctionNode::FunctionCallback callback) {
        return arena.make<FunctionNode>(name, expectedArgCount, args, callback);
    }
};

}  // namespace Expression

#endif  // EXPR_HELPER_H
