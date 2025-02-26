#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "memory/expr_arena.h"
#include "helpers/expr_helper.h"

#include "tracing/trace.h"

using namespace Expression;

void runEvaluationExample() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example ===\n";

    try {
        ExprArena arena;
        ExprHelper e(arena);

        // Define the variable environment.
        Env env;
        env["x"] = 3.14159265359; // Approximate Pi
        env["y"] = 2.0;

        // Expression: (sin(x) + y) * log_2(x) / ln(y)
        Node* expr = e.div(
            e.mul(
                e.add(e.sin(e.var("x")), e.var("y")),
                e.log(e.num(2), e.var("x"))
            ),
            e.ln(e.var("y"))
        );

        double result = expr->evaluate(env);

        std::cout << "Expression: " << expr->toString() << std::endl;
        std::cout << "Result: " << result << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error during evaluation: " << e.what() << std::endl;
    }
}

void runSimplificationExample() {
    Trace::clear();
    std::cout << "\n=== Simplification Example ===\n";

    try {
        ExprArena arena;
        ExprHelper e(arena);

        // Expression: ((x + 0) * (1 * y)) / (x^0 + y^1)
        Node* expr = e.div(
            e.mul(
                e.add(e.var("x"), e.num(0)),
                e.mul(e.num(1), e.var("y"))
            ),
            e.add(
                e.exp(e.var("x"), e.num(0)),
                e.exp(e.var("y"), e.num(1))
            )
        );

        Node* simplified = expr->simplify();

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Simplified Expression: " << simplified->toString() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error during simplification: " << e.what() << std::endl;
    }
}

void runDifferentiationExample() {
    Trace::clear();
    std::cout << "\n=== Differentiation Example ===\n";

    try {
        ExprArena arena;
        ExprHelper e(arena);

        // Expression: d/dx (x^2 * sin(x))
        Node* expr = e.mul(
            e.exp(e.var("x"), e.num(2)),
            e.sin(e.var("x"))
        );

        Node* derivative = expr->derivative("x");

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Derivative: " << derivative->simplify()->toString() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error during differentiation: " << e.what() << std::endl;
    }
}

int main() {
    try {
        runEvaluationExample();
        runSimplificationExample();
        runDifferentiationExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
