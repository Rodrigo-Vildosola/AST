#ifndef EXPR_HELPER_H
#define EXPR_HELPER_H

#include "number_node.h"
#include "addition_node.h"
#include "subtraction_node.h"
#include "multiplication_node.h"
#include "division_node.h"
#include "exponentiation_node.h"
#include "sin_node.h"
#include "cos_node.h"
#include "ln_node.h"
#include "log_node.h"
#include "variable_node.h"
#include "equality_node.h"
#include "function_node.h"

namespace Expression {

// Create a NumberNode.
inline Node* num(double value) {
    return new NumberNode(value);
}
    
// Create a VariableNode.
inline Node* var(const std::string &name) {
    return new VariableNode(name);
}
    
// Create an AdditionNode.
inline Node* add(Node* left, Node* right) {
    return new AdditionNode(left, right);
}
    
// Create a SubtractionNode.
inline Node* sub(Node* left, Node* right) {
    return new SubtractionNode(left, right);
}
    
// Create a MultiplicationNode.
inline Node* mul(Node* left, Node* right) {
    return new MultiplicationNode(left, right);
}
    
// Create a DivisionNode.
inline Node* div(Node* left, Node* right) {
    return new DivisionNode(left, right);
}
    
// Create an ExponentiationNode.
inline Node* exp(Node* base, Node* exponent) {
    return new ExponentiationNode(base, exponent);
}
    
// Create a SinNode.
inline Node* sin(Node* operand) {
    return new SinNode(operand);
}
    
// Create a CosNode.
inline Node* cos(Node* operand) {
    return new CosNode(operand);
}
    
// Create a LnNode (natural logarithm).
inline Node* ln(Node* operand) {
    return new LnNode(operand);
}
    
// Create a LogNode (logarithm with arbitrary base).
inline Node* log(Node* base, Node* operand) {
    return new LogNode(base, operand);
}
    
// Create an EqualityNode.
inline Node* eq(Node* left, Node* right) {
    return new EqualityNode(left, right);
}
    
// Create a FunctionNode.
inline Node* func(const std::string &name, int expectedArgCount, const std::vector<Node*>& args,
                  FunctionNode::FunctionCallback callback) {
    return new FunctionNode(name, expectedArgCount, args, callback);
}

} // namespace Expression

#endif // EXPR_HELPER_H
