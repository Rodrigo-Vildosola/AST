#include "memory/arena.h"
#include "memory/arena_allocator.h"
#include "memory/default_allocator.h"
#include "helpers/arena_node_factory.h"
#include "helpers/expr_operators.h"
#include "helpers/builder.h"
#include "helpers/expr.h"
#include "rewriting/rewriter.h"
#include "solver/solver.h"
#include "solver/token.h"

#include "tracing/trace.h"

using namespace Expression;

void runSolveExample() {
    std::cout << "\n=== Solve Higher Order Polynomial Example ===\n";

    try {
        // Use the Arena Allocator for full arena management.
        DECLARE_ARENA_FACTORY(f);

        // Build an equation: 4 * x ^ 4 + 3 * x ^ 3 + 2 * x ^ 2 == 10  (i.e., 4 * x ^ 4 + 3 * x ^ 3 + 2 * x ^ 2 - 10 = 0)
        Node* left = f.add(
            f.mul(f.exp(f.var("x"), f.num(4)), f.num(4)),
            f.add(
                f.mul(f.exp(f.var("x"), f.num(3)), f.num(3)),
                f.mul(f.exp(f.var("x"), f.num(2)), f.num(2))
            )
        );

        Node* right = f.num(10);

        Expr eq = Expr(left, f) == Expr(right, f);

        Expr x = Expr::Var("x", f);
        Expr equation = 4 * (x ^ 4) + 3 * (x ^ 3) + 2 * (x ^ 2) == 10;
        std::cout << "Equation: " << eq.toString() << std::endl;
        std::cout << "Equation: " << equation.toString() << std::endl;

        auto roots = eq.solveFor("x");
        std::cout << "Real Roots:" << std::endl;
        for (double r : roots) {
            std::cout << "x = " << r << std::endl;
        }

        auto roots2 = equation.solveFor("x");
        std::cout << "Real Roots:" << std::endl;
        for (double r : roots2) {
            std::cout << "x = " << r << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error solving polynomial: " << e.what() << std::endl;
    }
}


void runExprExample() {
    std::cout << "\n=== Try the new Expr helper Example ===\n";

    try {
        DECLARE_ARENA_FACTORY(f);

        Expr x = Expr::Var("x", f);
        Expr eq = -2 * x + 5 == 20;

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


void runTokenExample() {
    std::cout << "\n=== Try the new Builder helper Example ===\n";

    try {
        DECLARE_ARENA_FACTORY(f);

        std::vector<Token> tokens = {
            {TokenType::Variable, "x"},      // x
            {TokenType::Number,   "", 4},    // 4
            {TokenType::Operator, "^"},      // ^
            {TokenType::Number,   "", 4},    // 4
            {TokenType::Operator, "*"},      // *

            {TokenType::Variable, "x"},      // x
            {TokenType::Number,   "", 3},    // 3
            {TokenType::Operator, "^"},      // ^
            {TokenType::Number,   "", 3},    // 3
            {TokenType::Operator, "*"},      // *
            {TokenType::Operator, "+"},      // +

            {TokenType::Variable, "x"},      // x
            {TokenType::Number,   "", 2},    // 2
            {TokenType::Operator, "^"},      // ^
            {TokenType::Number,   "", 2},    // 2
            {TokenType::Operator, "*"},      // *
            {TokenType::Operator, "+"},      // +

            {TokenType::Number,   "", 10},   // 10
            {TokenType::Operator, "=="},     // ==
        };

        Expr eq = Expr(buildAstFromPostfix(tokens, f), f);

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
        runTokenExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
