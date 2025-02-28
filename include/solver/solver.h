// solver/solver.h
#pragma once

#include "polynomial_solver.h"
#include "expression/equality_node.h"
#include "helpers/node_factory.h"
#include "rewriting/rewriter.h"

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
            return simplified_diff->clone(factory);  // Or throw an error.
        }
        double solution = -b / a;
        Node* solNode = factory.num(solution);
        return solNode;
    }

    /// Solve an equation using the rewriting engine to normalize the equation first.
    static Node* solve(EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
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

    /// Solve a polynomial equation for the given variable.
    /// Assumes the equation is of the form f(x) == g(x).
    /// This function normalizes the equation, extracts polynomial coefficients,
    /// and then calls a polynomial solver for degree 1 or 2.
    static std::vector<double> solve_polynomial_equation(const EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
        // Convert f(x) == g(x) into f(x) - g(x) == 0.
        Node* diff = factory.sub(eq->left->clone(factory), eq->right->clone(factory));
        Node* simplified_diff = diff->simplify(factory);
        // delete diff; // free temporary node

        // Use our helper to extract polynomial coefficients.
        auto poly = extract_poly(simplified_diff, variable);
        auto coeffs = poly_to_vector(poly);

        // Solve the polynomial.
        return solve_polynomial(coeffs);
    }
};

class ExtendedSolver {
public:
    /// Solve an equation for a variable.
    /// If the normalized equation can be interpreted as a polynomial in the variable,
    /// this method extracts its coefficients and uses the polynomial solver.
    /// Otherwise, it falls back to linear solving or throws an error.
    static std::vector<double> solve_equation(const EqualityNode* eq, const std::string &variable, NodeFactory &factory) {
        Node* diff = factory.sub(eq->left, eq->right);
        // Node* simplified_diff = diff->simplify(factory);
        
        // Rewriter rewriter;
        // Node* normalized = rewriter.rewrite(simplified_diff, factory);

        std::cout << "Normalized Equation: " << diff->toString() << std::endl;

        auto poly = extract_poly(diff, variable);
        auto coeffs = poly_to_vector(poly);

        std::ostringstream oss;
        for (size_t i = 0; i < coeffs.size(); i++) {
            oss << "x^" << i << ": " << coeffs[i] << " ";
        }

        std::vector<double> roots = solve_polynomial(coeffs);

        return roots;
    }
};

} // namespace Expression
