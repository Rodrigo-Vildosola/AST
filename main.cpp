#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "expression/expr_helper.h"

#include "tracing/trace.h"

using namespace Expression;

void runEvaluationExample() {
    Trace::clear();
    std::cout << "\n=== Evaluation Example ===\n";

    try {
        // Define the variable environment.
        Env env;
        env["x"] = 3.14159265359; // Approximate Pi
        env["y"] = 2.0;

        // Expression: (sin(x) + y) * log_2(x) / ln(y)
        Node* expr = div(
            mul(
                add(sin(var("x")), var("y")),
                log(num(2), var("x"))
            ),
            ln(var("y"))
        );

        double result = expr->evaluate(env);

        std::cout << "Expression: " << expr->toString() << std::endl;
        std::cout << "Result: " << result << std::endl;

        delete expr;
    } catch (const std::exception &e) {
        std::cerr << "Error during evaluation: " << e.what() << std::endl;
    }
}

void runSimplificationExample() {
    Trace::clear();
    std::cout << "\n=== Simplification Example ===\n";

    try {
        // Expression: ((x + 0) * (1 * y)) / (x^0 + y^1)
        Node* expr = div(
            mul(
                add(var("x"), num(0)),
                mul(num(1), var("y"))
            ),
            add(
                exp(var("x"), num(0)),
                exp(var("y"), num(1))
            )
        );

        Node* simplified = expr->simplify();

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Simplified Expression: " << simplified->toString() << std::endl;

        delete expr;
        delete simplified;
    } catch (const std::exception &e) {
        std::cerr << "Error during simplification: " << e.what() << std::endl;
    }
}

void runDifferentiationExample() {
    Trace::clear();
    std::cout << "\n=== Differentiation Example ===\n";

    try {
        // Expression: d/dx (x^2 * sin(x))
        Node* expr = mul(
            exp(var("x"), num(2)),
            sin(var("x"))
        );

        Node* derivative = expr->derivative("x");

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Derivative: " << derivative->simplify()->toString() << std::endl;

        delete expr;
        delete derivative;
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
