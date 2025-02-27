// solver/solver.h
#pragma once

#include "expression/equality_node.h"
#include "helpers/node_factory.h"
#include "rewriting/rewriter.h"
#include "tracing/trace.h"
#include <stdexcept>
#include <cmath>

namespace Expression {

class Solver {
public:
    /// Solve a linear equation for the given variable.
    /// Assumes the equation is of the form f(x) == g(x).
    static Node* solve_linear(const EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
        // Convert equation f(x)==g(x) to f(x)-g(x)==0.
        Node* diff = factory.sub(eq->left->clone(factory), eq->right->clone(factory));
        Node* simplified_diff = diff->simplify(factory);

        double a = 0, b = 0;
        if (!simplified_diff->extractLinearCoeffs(variable, a, b) || a == 0) {
            Trace::addTransformation("Solving equation", simplified_diff->toString(), "Unable to solve linearly");
            return simplified_diff->clone(factory);  // Or throw an error.
        }
        double solution = -b / a;
        Node* solNode = factory.num(solution);
        Trace::addTransformation("Solving equation", simplified_diff->toString(), solNode->toString());
        return solNode;
    }

    /// Solve an equation using the rewriting engine to normalize the equation first.
    static Node* solve(const EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
        Rewriter rewriter;
        // First, use the rewriter to transform the equation.
        Node* rewritten = rewriter.rewrite(eq, factory);
        // Now assume rewritten is an EqualityNode or can be interpreted as one.
        // Here, for simplicity, we assume it is still an EqualityNode.
        if (auto eqNode = dynamic_cast<EqualityNode*>(rewritten)) {
            Node* sol = solve_linear(eqNode, variable, factory);
            return sol;
        } else {
            throw std::runtime_error("Rewritten expression is not an equation");
        }
    }
};

} // namespace Expression
