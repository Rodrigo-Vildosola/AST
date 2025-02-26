#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "memory/arena.h"
#include "memory/arena_allocator.h"
#include "memory/default_allocator.h"
#include "helpers/node_factory.h"

#include "tracing/trace.h"

using namespace Expression;

void runEvaluationExample() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example ===\n";

    try {
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, DeletionPolicy::Arena);

        // Define the variable environment.
        Env env;
        env["x"] = 3.14159265359; // Approximate Pi
        env["y"] = 2.0;

        // Expression: (sin(x) + y) * log_2(x) / ln(y)
        Node* expr = f.div(
            f.mul(
                f.add(f.sin(f.var("x")), f.var("y")),
                f.log(f.num(2), f.var("x"))
            ),
            f.ln(f.var("y"))
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
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, DeletionPolicy::Arena);

        // Expression: ((x + 0) * (1 * y)) / (x^0 + y^1)
        Node* expr = f.div(
            f.mul(
                f.add(f.var("x"), f.num(0)),
                f.mul(f.num(1), f.var("y"))
            ),
            f.add(
                f.exp(f.var("x"), f.num(0)),
                f.exp(f.var("y"), f.num(1))
            )
        );

        Node* simplified = expr->simplify(f);

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
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, DeletionPolicy::Arena);

        // Expression: d/dx (x^2 * sin(x))
        Node* expr = f.mul(
            f.exp(f.var("x"), f.num(2)),
            f.sin(f.var("x"))
        );

        Node* derivative = expr->derivative("x", f);

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Derivative: " << derivative->simplify(f)->toString() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error during differentiation: " << e.what() << std::endl;
    }
}


void runSolveExample() {
    Trace::clear();
    std::cout << "\n=== Solve Equation Example ===\n";

    try {
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, DeletionPolicy::Arena);

        // Create a simple equation: x == 3
        // (This means: the expression is an equality node with left side 'x' and right side '3')
        Node* equation = f.eq(
            f.add(f.mul(f.num(2), f.var("x")), f.num(5)),  // Left: (2 * x) + 5
            f.num(11)                                      // Right: 11
        );

        arena.printStats();

        if (EqualityNode* eq = dynamic_cast<EqualityNode*>(equation)) {
            Node* solution = eq->solveFor("x", f);

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


void runEvalExampleArena() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example (Arena Allocator) ===\n";

    Arena arena;
    ArenaAllocator arenaAlloc(arena);
    NodeFactory f(&arenaAlloc, DeletionPolicy::Arena);

    Env env;
    env["x"] = 3.14159265359;
    env["y"] = 2.0;

    Node* expr = f.div(
        f.mul(
            f.add(f.sin(f.var("x")), f.var("y")),
            f.log(f.num(2), f.var("x"))
        ),
        f.ln(f.var("y"))
    );

    double result = expr->evaluate(env);

    std::cout << "Expression: " << expr->toString() << std::endl;
    std::cout << "Result: " << result << std::endl;
}

void runEvalExampleDefault() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example (Default Allocator) ===\n";

    DefaultAllocator defaultAlloc;
    NodeFactory f(&defaultAlloc, DeletionPolicy::Default);

    Env env;
    env["x"] = 3.14159265359;
    env["y"] = 2.0;

    Node* expr = f.div(
        f.mul(
            f.add(f.sin(f.var("x")), f.var("y")),
            f.log(f.num(2), f.var("x"))
        ),
        f.ln(f.var("y"))
    );

    double result = expr->evaluate(env);

    std::cout << "Expression: " << expr->toString() << std::endl;
    std::cout << "Result: " << result << std::endl;

    // Clean up manually if using DefaultAllocator.
    delete expr;
}

int main() {
    try {
        // runEvalExampleArena();
        // runEvalExampleDefault();

        runSolveExample();

        // runEvaluationExample();
        // runSimplificationExample();
        // runDifferentiationExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
