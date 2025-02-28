#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "memory/arena.h"
#include "memory/arena_allocator.h"
#include "memory/default_allocator.h"
#include "helpers/node_factory.h"
#include "rewriting/rewriter.h"
#include "solver/solver.h"

#include "tracing/trace.h"

using namespace Expression;

void runEvaluationExample() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example ===\n";

    try {
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

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
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

        // Expression: ((x + 0) * (1 * y)) / (x^0 + y^1)
        // Node* expr = f.div(
        //     f.mul(
        //         f.add(f.var("x"), f.num(0)),
        //         f.mul(f.num(1), f.var("y"))
        //     ),
        //     f.add(
        //         f.exp(f.var("x"), f.num(0)),
        //         f.exp(f.var("y"), f.num(1))
        //     )
        // );

        Node* expr = f.mul(
            f.add(f.num(10), f.num(5)),
            f.num(4)
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
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

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
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

        // Create a simple equation: x == 3
        // (This means: the expression is an equality node with left side 'x' and right side '3')
        Node* equation = f.eq(
            f.add(f.mul(f.num(2), f.var("x")), f.num(5)),
            f.num(20)
        );

        std::cout << "Original Equation: " << equation->toString() << std::endl;

        // Use the rewriting engine to further normalize the equation.
        Rewriter rewriter;
        Node* rewritten_eq = rewriter.rewrite(equation, f);
        std::cout << "Rewritten Equation: " << rewritten_eq->toString() << std::endl;

        // Solve the equation using our Solver module.
        if (auto eqNode = dynamic_cast<EqualityNode*>(equation)) {
            Node* solution = Solver::solve_linear(eqNode, "x", f);
            std::cout << "Solution for x: " << solution->toString() << std::endl;
            // Note: When using an arena, you do not manually delete nodes.
        } else {
            std::cerr << "Not an equation!" << std::endl;
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
    NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

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
    NodeFactory f(&defaultAlloc, AllocatorPolicy::Default);

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

void runSolvePolynomialExample() {
    Trace::clear();
    std::cout << "\n=== Solve Higher Order Polynomial Example ===\n";

    try {
        // Use the Arena Allocator for full arena management.
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        // Create a NodeFactory that uses the arena allocator.
        NodeFactory factory(&arenaAlloc, AllocatorPolicy::Arena);

        // Build an equation: x^3 == 27  (i.e., x^3 - 27 = 0)
        Node* equation = factory.eq(
            factory.exp(factory.var("x"), factory.num(4)),
            factory.num(9)
        );

        std::cout << "Equation: " << equation->toString() << std::endl;

        // Use the extended solver to extract polynomial coefficients and solve.
        // Our current implementation supports only up to quadratic equations.
        auto roots = Solver::solve_polynomial_equation(dynamic_cast<EqualityNode*>(equation), "x", factory);
        std::cout << "Polynomial Roots:" << std::endl;
        for (double r : roots) {
            std::cout << "  x = " << r << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error solving polynomial: " << e.what() << std::endl;
    }
}

void runRewriteExample() {
    Trace::clear();
    std::cout << "\n=== Rewrite Equation Example ===\n";

    try {
        Arena arena;
        ArenaAllocator arenaAlloc(arena);
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

        // Create a simple equation: x == 3
        // (This means: the expression is an equality node with left side 'x' and right side '3')
        Node* equation = f.mul(
            f.exp(f.num(10), f.num(2)),
            f.exp(f.num(10), f.num(3))
        );

        std::cout << "Original Equation: " << equation->toString() << std::endl;

        // Use the rewriting engine to further normalize the equation.
        Rewriter rewriter;
        Node* rewritten_eq = rewriter.rewrite(equation, f);
        std::cout << "Rewritten Equation: " << rewritten_eq->toString() << std::endl;

        // Solve the equation using our Solver module.
        // if (auto eqNode = dynamic_cast<EqualityNode*>(equation)) {
        //     Node* solution = Solver::solve_linear(eqNode, "x", f);
        //     std::cout << "Solution for x: " << solution->toString() << std::endl;
        //     // Note: When using an arena, you do not manually delete nodes.
        // } else {
        //     std::cerr << "Not an equation!" << std::endl;
        // }

        // Note: Do not manually delete 'equation' or 'solution' when using the arena allocator.
        // The arena will take care of deallocation when it goes out of scope.
    } catch (const std::exception &e) {
        std::cerr << "Error during solving: " << e.what() << std::endl;
    }
}


int main() {
    try {
        // runEvalExampleArena();
        // runEvalExampleDefault();

        runSolveExample();
        runRewriteExample();
        runSolvePolynomialExample();
        // runEvaluationExample();
        // runSimplificationExample();
        // runDifferentiationExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
