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
        NodeFactory f(&arenaAlloc, AllocatorPolicy::Arena);

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


int main() {
    try {
        runSolveExample();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
    }
    return 0;
}
