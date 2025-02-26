#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "memory/expr_arena.h"
#include "helpers/node_factory.h"

#include "tracing/trace.h"

using namespace Expression;

void runSolveExample() {
    Trace::clear();
    std::cout << "\n=== Solve Equation Example ===\n";

    try {
        ExprArena arena;
        NodeFactory e(arena);

        // Create a simple equation: x == 3
        // (This means: the expression is an equality node with left side 'x' and right side '3')
        Node* equation = e.eq(
            e.add(e.mul(e.num(2), e.var("x")), e.num(5)),  // Left: (2 * x) + 5
            e.num(11)                                      // Right: 11
        );

        arena.printStats();

        if (EqualityNode* eq = dynamic_cast<EqualityNode*>(equation)) {
            Node* solution = eq->solveFor("x", e);

            std::cout << "Equation: " << equation->toString() << std::endl;
            std::cout << "Solution for x: " << solution->toString() << std::endl;
        } else {
            throw std::runtime_error("Not an equation");
        }
        // Note: Do not manually delete 'equation' or 'solution' when using the arena allocator.
        // The arena will take care of deallocation when it goes out of scope.
    } catch (const std::exception &e) {
        std::cerr << "Error during solving: " << e.what() << std::endl;
    }
}

void runEvaluationExample() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example ===\n";

    try {
        ExprArena arena;
        NodeFactory e(arena);

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
        NodeFactory e(arena);

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

        Node* simplified = expr->simplify(e);

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
        NodeFactory e(arena);

        // Expression: d/dx (x^2 * sin(x))
        Node* expr = e.mul(
            e.exp(e.var("x"), e.num(2)),
            e.sin(e.var("x"))
        );

        Node* derivative = expr->derivative("x", e);

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Derivative: " << derivative->simplify(e)->toString() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error during differentiation: " << e.what() << std::endl;
    }
}

int main() {
    try {
        runSolveExample();

        runEvaluationExample();
        runSimplificationExample();
        runDifferentiationExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
