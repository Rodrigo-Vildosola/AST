#include "expression/sin_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/cos_node.h"
#include "helpers/node_factory.h"
#include "tracing/trace.h"

namespace Expression {

SinNode::SinNode(Node* operand)
    : UnaryOpNode(operand) {}

SinNode::~SinNode() {
    // no manual deletion
}

double SinNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::sin(opVal);
    Trace::addTransformation("Evaluating SinNode", toString(), std::to_string(result));
    return result;
}

std::string SinNode::toString() const {
    return "sin(" + operand->toString() + ")";
}

// **Simplification**
Node* SinNode::simplify(NodeFactory &factory) const {
    std::string before = toString();
    Node* simplifiedOperand = operand->simplify(factory);
    Node* result = factory.sin(simplifiedOperand);
    Trace::addTransformation("Simplify SinNode", before, result->toString());
    return result;
}

// **Differentiation (d/dx sin(x) = cos(x)*dx)**
Node* SinNode::derivative(const std::string& variable, NodeFactory &factory) const {
    std::string before = toString();
    // cos(operand)* operand->derivative(variable)
    Node* cosTerm = factory.cos(operand->clone(factory));
    Node* opDeriv = operand->derivative(variable, factory);
    Node* derivativeResult = factory.mul(cosTerm, opDeriv);

    Trace::addTransformation("Differentiate SinNode", before, derivativeResult->toString());
    return derivativeResult;
}

// **Substitution**
Node* SinNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    std::string before = toString();
    Node* newOperand = operand->substitute(variable, value, factory);
    Node* result = factory.sin(newOperand);
    Trace::addTransformation("Substituting in SinNode", before, result->toString());
    return result;
}

// **Clone**
Node* SinNode::clone(NodeFactory &factory) const {
    return factory.sin(operand->clone(factory));
}

bool SinNode::equals(const Node* other) const {
    if (const SinNode* sinNode = dynamic_cast<const SinNode*>(other)) {
        return operand->equals(sinNode->operand);
    }
    return false;
}

bool SinNode::extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const {
    double a = 0, b = 0;
    if (operand->extractLinearCoeffs(var, a, b)) {
        if (a == 0) {  // Operand is constant.
            coeff = 0;
            constant = std::sin(b);
            return true;
        }
    }
    return false;
}


} // namespace Expression
