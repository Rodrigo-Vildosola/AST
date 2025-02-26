#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "expression/number_node.h"
#include "expression/addition_node.h"
#include "expression/subtraction_node.h"
#include "expression/multiplication_node.h"
#include "expression/division_node.h"
#include "expression/exponentiation_node.h"
#include "expression/sin_node.h"
#include "expression/cos_node.h"
#include "expression/ln_node.h"
#include "expression/log_node.h"
#include "expression/variable_node.h"
#include "expression/equality_node.h"
#include "expression/function_node.h"
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

        // Create an expression: (sin(x) + y) * log_2(x) / ln(y)
        Node* var_x = new VariableNode("x");
        Node* var_y = new VariableNode("y");

        Node* sin_x = new SinNode(var_x);
        Node* log_x_base2 = new LogNode(new NumberNode(2), var_x);
        Node* ln_y = new LnNode(var_y);
        Node* sum_expr = new AdditionNode(sin_x, var_y);
        Node* multiplication = new MultiplicationNode(sum_expr, log_x_base2);
        Node* final_expr = new DivisionNode(multiplication, ln_y);

        double result = final_expr->evaluate(env);

        std::cout << "Expression: " << final_expr->toString() << std::endl;
        std::cout << "Result: " << result << std::endl;
        std::cout << "\nEvaluation Trace:\n" << Trace::getTrace() << std::endl;

        delete final_expr;
    } catch (const std::exception &e) {
        std::cerr << "Error during evaluation: " << e.what() << std::endl;
    }
}

void runDifferentiationExample() {
    Trace::clear();
    std::cout << "\n=== Differentiation Example ===\n";

    try {
        // Create an expression: d/dx (x^2 * sin(x))
        Node* var_x = new VariableNode("x");
        Node* exponentiation = new ExponentiationNode(var_x, new NumberNode(2));
        Node* sin_x = new SinNode(var_x);
        Node* expr = new MultiplicationNode(exponentiation, sin_x);

        Node* derivative = expr->derivative("x");

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Derivative: " << derivative->toString() << std::endl;
        std::cout << "\nDifferentiation Trace:\n" << Trace::getTrace() << std::endl;

        delete expr;
        delete derivative;
    } catch (const std::exception &e) {
        std::cerr << "Error during differentiation: " << e.what() << std::endl;
    }
}

void runSimplificationExample() {
    Trace::clear();
    std::cout << "\n=== Simplification Example ===\n";

    try {
        // Create an expression: ((x + 0) * (1 * y)) / (x^0 + y^1)
        Node* var_x = new VariableNode("x");
        Node* var_y = new VariableNode("y");

        Node* addition = new AdditionNode(var_x, new NumberNode(0));
        Node* multiplication = new MultiplicationNode(new NumberNode(1), var_y);
        Node* power_x = new ExponentiationNode(var_x, new NumberNode(0));
        Node* power_y = new ExponentiationNode(var_y, new NumberNode(1));
        Node* denominator = new AdditionNode(power_x, power_y);
        Node* expr = new DivisionNode(new MultiplicationNode(addition, multiplication), denominator);

        Node* simplified = expr->simplify();

        std::cout << "Original Expression: " << expr->toString() << std::endl;
        std::cout << "Simplified Expression: " << simplified->toString() << std::endl;
        std::cout << "\nSimplification Trace:\n" << Trace::getTrace() << std::endl;

        delete expr;
        delete simplified;
    } catch (const std::exception &e) {
        std::cerr << "Error during simplification: " << e.what() << std::endl;
    }
}

int main() {
    try {
        runEvaluationExample();
        runDifferentiationExample();
        runSimplificationExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
