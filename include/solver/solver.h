#pragma once

#include "expression/equality_node.h"
#include "helpers/node_factory.h"

namespace Expression {

class Solver {
public:
    // Solve a linear equation for the given variable.
    // The equation is assumed to be of the form f(x) == 0.
    static Node* solveLinear(const EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
        // Convert the equation f(x) = g(x) into f(x) - g(x) = 0.
        Node* diff = factory.sub(eq->left->clone(factory), eq->right->clone(factory));
        Node* simplifiedDiff = diff->simplify(factory);
        delete diff; // Free temporary difference if needed.

        double a = 0, b = 0;
        // Try to extract coefficients so that simplifiedDiff is a*x + b.
        if (!simplifiedDiff->extractLinearCoeffs(variable, a, b) || a == 0) {
            Trace::addTransformation("Solving equation", simplifiedDiff->toString(), "Unable to solve linearly");
            return simplifiedDiff->clone(factory); // Or return nullptr.
        }
        double solution = -b / a;
        Node* solNode = factory.num(solution);
        Trace::addTransformation("Solving equation", simplifiedDiff->toString(), solNode->toString());
        return solNode;
    }
};

} // namespace Expression
