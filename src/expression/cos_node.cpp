#include "expression/cos_node.h"
#include "expression/number_node.h"
#include "expression/multiplication_node.h"
#include "expression/sin_node.h"
#include "helpers/node_factory.h"  // NodeFactory
#include "tracing/trace.h"

namespace Expression {

CosNode::CosNode(Node* operand)
    : UnaryOpNode(operand) {}

CosNode::~CosNode() {}

double CosNode::evaluate(const Env &env) {
    double opVal = operand->evaluate(env);
    double result = std::cos(opVal);
    Trace::addTransformation("Evaluating CosNode", toString(), std::to_string(result));
    return result;
}

std::string CosNode::toString() const {
    return "cos(" + operand->toString() + ")";
}

// **Simplification (Full Arena)**
Node* CosNode::simplify(NodeFactory &factory) const {
    std::string before = toString();
    Node* simplifiedOperand = operand->simplify(factory);
    Node* result = factory.cos(simplifiedOperand);
    Trace::addTransformation("Simplify CosNode", before, result->toString());
    return result;
}

// **Differentiation** d/dx cos(x) = -sin(x) * dx
Node* CosNode::derivative(const std::string& variable, NodeFactory &factory) const {
    std::string before = toString();
    // -sin(operand) * operand->derivative
    Node* negativeOne = factory.num(-1);
    Node* sinTerm = factory.sin(operand->clone(factory));
    Node* derivTerm = operand->derivative(variable, factory);
    // multiply them: (-1) * ( sin(...) * deriv(...) )
    Node* mulInner = factory.mul(sinTerm, derivTerm);
    Node* derivativeResult = factory.mul(negativeOne, mulInner);

    Trace::addTransformation("Differentiate CosNode", before, derivativeResult->toString());
    return derivativeResult;
}

// **Substitution**
Node* CosNode::substitute(const std::string& variable, Node* value, NodeFactory &factory) const {
    std::string before = toString();
    Node* substitutedOperand = operand->substitute(variable, value, factory);
    Node* result = factory.cos(substitutedOperand);
    Trace::addTransformation("Substituting in CosNode", before, result->toString());
    return result;
}

// **Clone**
Node* CosNode::clone(NodeFactory &factory) const {
    return factory.cos(operand->clone(factory));
}

bool CosNode::extractLinearCoeffs(const std::string &var, double &coeff, double &constant) const {
    double a = 0, b = 0;
    if (operand->extractLinearCoeffs(var, a, b)) {
        if (a == 0) {  // Operand is constant.
            coeff = 0;
            constant = std::cos(b);
            return true;
        }
    }
    return false;
}


} // namespace Expression
