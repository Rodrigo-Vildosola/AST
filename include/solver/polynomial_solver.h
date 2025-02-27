#pragma once

#include "expression/node.h"
#include "expression/number_node.h"
#include "expression/variable_node.h"
#include "expression/addition_node.h"
#include "expression/subtraction_node.h"
#include "expression/multiplication_node.h"
#include "expression/exponentiation_node.h"


namespace Expression {

    // A simple polynomial is represented as a mapping from exponent to coefficient.
    using Poly = std::map<int, double>;

    // Helper to add two polynomials.
    inline Poly add_poly(const Poly &p1, const Poly &p2) {
        Poly result = p1;
        for (const auto &entry : p2) {
            result[entry.first] += entry.second;
        }
        return result;
    }

    // Helper to subtract p2 from p1.
    inline Poly sub_poly(const Poly &p1, const Poly &p2) {
        Poly result = p1;
        for (const auto &entry : p2) {
            result[entry.first] -= entry.second;
        }
        return result;
    }

    // Helper to multiply two polynomials.
    inline Poly mul_poly(const Poly &p1, const Poly &p2) {
        Poly result;
        for (const auto &e1 : p1) {
            for (const auto &e2 : p2) {
                result[e1.first + e2.first] += e1.second * e2.second;
            }
        }
        return result;
    }

    // Recursively extract polynomial coefficients from a Node.
    // Assumes that the expression is expanded and simplified.
    inline Poly extract_poly(const Node* node, const std::string &var) {
        Poly poly;
        // If the node is a constant.
        if (auto num = dynamic_cast<const NumberNode*>(node)) {
            poly[0] = num->getValue();
            return poly;
        }
        // If the node is the variable.
        if (auto varNode = dynamic_cast<const VariableNode*>(node)) {
            if (varNode->toString() == var) {
                poly[1] = 1.0;
                return poly;
            }
            // If it's a different variable, treat it as constant (or throw an error).
            poly[0] = 0;
            return poly;
        }
        // If the node is an addition.
        if (auto add = dynamic_cast<const AdditionNode*>(node)) {
            Poly p1 = extract_poly(add->left, var);
            Poly p2 = extract_poly(add->right, var);
            return add_poly(p1, p2);
        }
        // If the node is a subtraction.
        if (auto sub = dynamic_cast<const SubtractionNode*>(node)) {
            Poly p1 = extract_poly(sub->left, var);
            Poly p2 = extract_poly(sub->right, var);
            return sub_poly(p1, p2);
        }
        // If the node is a multiplication.
        if (auto mul = dynamic_cast<const MultiplicationNode*>(node)) {
            Poly p1 = extract_poly(mul->left, var);
            Poly p2 = extract_poly(mul->right, var);
            return mul_poly(p1, p2);
        }
        // If the node is an exponentiation.
        if (auto exp = dynamic_cast<const ExponentiationNode*>(node)) {
            // Only support constant exponent for now.
            if (auto expNum = dynamic_cast<const NumberNode*>(exp->right)) {
                int power = static_cast<int>(std::round(expNum->getValue()));
                Poly basePoly = extract_poly(exp->left, var);
                Poly result;
                result[0] = 1;  // Initialize as 1.
                for (int i = 0; i < power; ++i) {
                    result = mul_poly(result, basePoly);
                }
                return result;
            }
        }
        // Otherwise, treat the node as constant (evaluate it).
        poly[0] = node->evaluate(Env());
        return poly;
    }

    // Convert polynomial (map) to a vector (coefficients in ascending order).
    inline std::vector<double> poly_to_vector(const Poly &poly) {
        int degree = poly.rbegin()->first;
        std::vector<double> coeffs(degree + 1, 0.0);
        for (const auto &entry : poly) {
            coeffs[entry.first] = entry.second;
        }
        return coeffs;
    }

    inline std::vector<double> solve_polynomial(const std::vector<double>& coeffs) {
        int degree = coeffs.size() - 1;
        std::vector<double> roots;
        if (degree == 1) {
            // Linear: a*x + b = 0 => x = -b / a
            if (std::fabs(coeffs[1]) < 1e-9) {
                throw std::runtime_error("No solution or infinite solutions for linear equation.");
            }
            roots.push_back(-coeffs[0] / coeffs[1]);
        } else if (degree == 2) {
            // Quadratic: a*x^2 + b*x + c = 0
            double a = coeffs[2], b = coeffs[1], c = coeffs[0];
            double discriminant = b * b - 4 * a * c;
            if (discriminant < 0) {
                // No real roots.
                return roots;
            }
            roots.push_back((-b + std::sqrt(discriminant)) / (2 * a));
            roots.push_back((-b - std::sqrt(discriminant)) / (2 * a));
        } else {
            throw std::runtime_error("Polynomial degree higher than 2 is not supported in this example.");
        }
        return roots;
    }
}
