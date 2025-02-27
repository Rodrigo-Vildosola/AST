#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include "memory/iallocator.h"
#include "memory/arena_allocator.h"
#include "memory/default_allocator.h"

#include "memory/allocation_config.h"


namespace Expression {

class NodeFactory {
private:
    IAllocator* allocator;
    // A cache mapping a canonical key to a node pointer.
    std::unordered_map<std::string, Node*> nodeCache;

    // Helper functions to build canonical keys for different node types.
    std::string keyForNumber(double value) {
        return "num(" + std::to_string(value) + ")";
    }

    std::string keyForVariable(const std::string &name) {
        return "var(" + name + ")";
    }

    std::string keyForBinary(const std::string &op, Node* left, Node* right) {
        return op + "(" + left->toString() + "," + right->toString() + ")";
    }

    std::string keyForUnary(const std::string &op, Node* operand) {
        return op + "(" + operand->toString() + ")";
    }

    std::string keyForFunction(const std::string &name, int expectedArgCount, const std::vector<Node*>& args) {
        std::ostringstream oss;
        oss << "func(" << name << "," << expectedArgCount << ",";
        for (auto arg : args) {
            oss << arg->toString() << ",";
        }
        oss << ")";
        return oss.str();
    }

public:
    NodeFactory(IAllocator* alloc, DeletionPolicy policy) : allocator(alloc) {
        AllocationConfig::setPolicy(policy);
    }

    Node* num(double value) {
        std::string key = keyForNumber(value);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createNumber(value);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* var(const std::string &name) {
        std::string key = keyForVariable(name);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createVariable(name);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* add(Node* left, Node* right) {
        std::string key = keyForBinary("add", left, right);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createAddition(left, right);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* sub(Node* left, Node* right) {
        std::string key = keyForBinary("sub", left, right);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createSubtraction(left, right);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* mul(Node* left, Node* right) {
        std::string key = keyForBinary("mul", left, right);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createMultiplication(left, right);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* div(Node* left, Node* right) {
        std::string key = keyForBinary("div", left, right);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createDivision(left, right);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* exp(Node* base, Node* exponent) {
        std::string key = keyForBinary("exp", base, exponent);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createExponentiation(base, exponent);
        nodeCache[key] = newNode;
        return newNode;
    }

    // Unary Operations with caching
    Node* sin(Node* operand) {
        std::string key = keyForUnary("sin", operand);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createSin(operand);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* cos(Node* operand) {
        std::string key = keyForUnary("cos", operand);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createCos(operand);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* ln(Node* operand) {
        std::string key = keyForUnary("ln", operand);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createLn(operand);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* log(Node* base, Node* operand) {
        std::string key = "log(" + base->toString() + "," + operand->toString() + ")";
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createLog(base, operand);
        nodeCache[key] = newNode;
        return newNode;
    }

    // Equality & Function Nodes with caching
    Node* eq(Node* left, Node* right) {
        std::string key = keyForBinary("eq", left, right);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createEquality(left, right);
        nodeCache[key] = newNode;
        return newNode;
    }

    Node* func(const std::string &name, int expectedArgCount, const std::vector<Node*>& args,
               FunctionCallback callback) {
        std::string key = keyForFunction(name, expectedArgCount, args);
        auto it = nodeCache.find(key);
        if (it != nodeCache.end()) {
            return it->second;
        }
        Node* newNode = allocator->createFunction(name, expectedArgCount, args, callback);
        nodeCache[key] = newNode;
        return newNode;
    }
};

} // namespace Expression

#endif // NODE_FACTORY_H
