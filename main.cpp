#include <iostream>
#include <unordered_map>
#include <vector>
#include "expression/number_node.h"
#include "expression/addition_node.h"
#include "expression/sin_node.h"
#include "expression/variable_node.h"
#include "expression/equality_node.h"
#include "expression/function_node.h"
#include "tracing/trace.h"

using namespace Expression;

int main() {
    // Clear the evaluation trace.
    Trace::clear();

    // Create an environment with variable definitions.
    Env env;
    env["x"] = 3.14159265359;
    env["y"] = 2.0;

    // Construct an expression: sin(x) + y
    Node* var_x = new VariableNode("x");
    Node* sin_x = new SinNode(var_x);       // sin(x)
    Node* var_y = new VariableNode("y");
    Node* expr1 = new AdditionNode(sin_x, var_y);  // sin(x) + y

    double result1 = expr1->evaluate(env);
    std::cout << "Expression 1: " << expr1->toString() << std::endl;
    std::cout << "Result 1: " << result1 << std::endl;

    // Construct an equality expression: (sin(x) + y) == 2.0
    // Note: For the sake of the example, we compare expr1 to the constant 2.0.
    Node* const_two = new NumberNode(2.0);
    Node* equalityExpr = new EqualityNode(expr1, const_two);

    double equalityResult = equalityExpr->evaluate(env);
    std::cout << "Equality Expression: " << equalityExpr->toString() << std::endl;
    std::cout << "Equality Result (1.0 means true, 0.0 means false): " << equalityResult << std::endl;

    // Construct a multi-argument function node: f(x, y) = x * y
    auto multiply = [](const std::vector<double>& args) -> double {
        return args[0] * args[1];
    };
    std::vector<Node*> funcArgs;
    funcArgs.push_back(new VariableNode("x"));
    funcArgs.push_back(new VariableNode("y"));
    Node* funcNode = new FunctionNode("mul", 2, funcArgs, multiply);

    double funcResult = funcNode->evaluate(env);
    std::cout << "Function Expression: " << funcNode->toString() << std::endl;
    std::cout << "Function Result: " << funcResult << std::endl;

    std::cout << "\nEvaluation Trace:\n" << Trace::getTrace() << std::endl;

    // Clean up memory.
    delete equalityExpr;
    delete funcNode;

    return 0;
}
