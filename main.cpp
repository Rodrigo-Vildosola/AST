#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "memory/arena.h"
#include "memory/arena_allocator.h"
#include "memory/default_allocator.h"
#include "helpers/arena_node_factory.h"
#include "helpers/expr.h"
#include "rewriting/rewriter.h"
#include "solver/solver.h"

#include "tracing/trace.h"

using namespace Expression;

void runSolveExample() {
    Trace::clear();
    std::cout << "\n=== Solve Higher Order Polynomial Example ===\n";

    try {
        // Use the Arena Allocator for full arena management.
        DECLARE_ARENA_FACTORY(f);

        // Build an equation: x^3 == 27  (i.e., x^3 - 27 = 0)
        Node* left = f.add(
            f.add(
                f.mul(f.exp(f.var("x"), f.num(3)), f.num(3)),
                f.mul(f.exp(f.var("x"), f.num(2)), f.num(2))
            ),
            f.mul(f.exp(f.var("x"), f.num(4)), f.num(4))
        );

        Node* right = f.num(10);

        Node* equation = f.eq(left, right);

        std::cout << "Equation: " << equation->toString() << std::endl;

        std::vector<double> roots = ExtendedSolver::solve_equation(dynamic_cast<const EqualityNode*>(equation), "x", f);
        std::cout << "Real Roots:" << std::endl;
        for (double r : roots) {
            std::cout << "x = " << r << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error solving polynomial: " << e.what() << std::endl;
    }
}


void runExprExample() {
    Trace::clear();
    std::cout << "\n=== Try the new Expr helper Example ===\n";

    try {
        DECLARE_ARENA_FACTORY(f);

        // Build an expression: 2*x + 5 == 20
        Expr x = Expr::Var("x", f);
        Expr eq = Expr::Num(2, f) * x + Expr::Num(5, f) == Expr::Num(20, f);

        std::cout << "Equation: " << eq.toString() << std::endl;

        // Solve for x.
        auto roots = eq.solveFor("x");
        std::cout << "Solution(s) for x:" << std::endl;
        for (double r : roots) {
            std::cout << "x = " << r << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error solving equation: " << e.what() << std::endl;
    }
}

int main() {
    try {
        runSolveExample();
        runExprExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
