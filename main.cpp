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

void runSolveExample() {
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
            factory.exp(factory.var("x"), factory.num(3)),
            factory.num(10)
        );

        std::cout << "Equation: " << equation->toString() << std::endl;

        std::vector<double> roots = ExtendedSolver::solve_equation(dynamic_cast<const EqualityNode*>(equation), "x", factory);
        std::cout << "Real Roots:" << std::endl;
        for (double r : roots) {
            std::cout << "x = " << r << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error solving polynomial: " << e.what() << std::endl;
    }
}


int main() {
    try {
        runSolveExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
