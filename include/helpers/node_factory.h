#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include "memory/iallocator.h"
#include "memory/allocation_config.h"


namespace Expression {

/**
 * @brief NodeFactory wraps an IAllocator to provide concise node-creation functions.
 * It supports multiple allocation strategies (e.g., arena vs. default).
 */
class NodeFactory {
private:
    IAllocator* allocator;
public:
    NodeFactory(IAllocator* alloc, DeletionPolicy policy)
        : allocator(alloc)
    {
        AllocationConfig::setPolicy(policy);
    }

    // Number & Variable Nodes
    Node* num(double value) { return allocator->createNumber(value); }
    Node* var(const std::string &name) { return allocator->createVariable(name); }

    // Binary Operations
    Node* add(Node* left, Node* right) { return allocator->createAddition(left, right); }
    Node* sub(Node* left, Node* right) { return allocator->createSubtraction(left, right); }
    Node* mul(Node* left, Node* right) { return allocator->createMultiplication(left, right); }
    Node* div(Node* left, Node* right) { return allocator->createDivision(left, right); }
    Node* exp(Node* base, Node* exponent) { return allocator->createExponentiation(base, exponent); }

    // Unary Operations
    Node* sin(Node* operand) { return allocator->createSin(operand); }
    Node* cos(Node* operand) { return allocator->createCos(operand); }
    Node* ln(Node* operand) { return allocator->createLn(operand); }
    Node* log(Node* base, Node* operand) { return allocator->createLog(base, operand); }

    // Equality & Functions
    Node* eq(Node* left, Node* right) { return allocator->createEquality(left, right); }
    Node* func(const std::string &name, int expectedArgCount, const std::vector<Node*>& args,
               FunctionCallback callback) {
        return allocator->createFunction(name, expectedArgCount, args, callback);
    }
};

} // namespace Expression

#endif // NODE_FACTORY_H
