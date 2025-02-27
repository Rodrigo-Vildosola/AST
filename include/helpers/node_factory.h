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
    bool cacheEnabled;  // Determines whether caching is active.
    // Cache mapping a canonical key to a node pointer.
    std::unordered_map<std::string, Node*> nodeCache;

    // Helper template function to reduce repetition.
    template<typename F>
    Node* getOrCreate(const std::string &key, F creator) {
        if (cacheEnabled) {
            auto it = nodeCache.find(key);
            if (it != nodeCache.end()) {
                return it->second;
            }
            Node* newNode = creator();
            nodeCache[key] = newNode;
            return newNode;
        } else {
            return creator();
        }
    }

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
    // The constructor accepts an IAllocator pointer, a deletion policy,
    // and an optional flag to enable caching (default: true).
    NodeFactory(IAllocator* alloc, AllocatorPolicy policy, bool enableCache = true)
        : allocator(alloc), cacheEnabled(enableCache)
    {
        AllocationConfig::setPolicy(policy);
        if (AllocationConfig::getPolicy() == AllocatorPolicy::Default) {
            cacheEnabled = false;
        }
    }

    // Setter to enable or disable caching at runtime.
    void setCacheEnabled(bool enable) {
        cacheEnabled = enable;
        if (!enable) {
            nodeCache.clear();
        }
    }

    // Number & Variable Nodes with caching
    Node* num(double value) {
        std::string key = keyForNumber(value);
        return getOrCreate(key, [this, value]() {
            return allocator->createNumber(value);
        });
    }

    Node* var(const std::string &name) {
        std::string key = keyForVariable(name);
        return getOrCreate(key, [this, name]() {
            return allocator->createVariable(name);
        });
    }

    // Binary Operations with caching
    Node* add(Node* left, Node* right) {
        std::string key = keyForBinary("add", left, right);
        return getOrCreate(key, [this, left, right]() {
            return allocator->createAddition(left, right);
        });
    }

    Node* sub(Node* left, Node* right) {
        std::string key = keyForBinary("sub", left, right);
        return getOrCreate(key, [this, left, right]() {
            return allocator->createSubtraction(left, right);
        });
    }

    Node* mul(Node* left, Node* right) {
        std::string key = keyForBinary("mul", left, right);
        return getOrCreate(key, [this, left, right]() {
            return allocator->createMultiplication(left, right);
        });
    }

    Node* div(Node* left, Node* right) {
        std::string key = keyForBinary("div", left, right);
        return getOrCreate(key, [this, left, right]() {
            return allocator->createDivision(left, right);
        });
    }

    Node* exp(Node* base, Node* exponent) {
        std::string key = keyForBinary("exp", base, exponent);
        return getOrCreate(key, [this, base, exponent]() {
            return allocator->createExponentiation(base, exponent);
        });
    }

    // Unary Operations with caching
    Node* sin(Node* operand) {
        std::string key = keyForUnary("sin", operand);
        return getOrCreate(key, [this, operand]() {
            return allocator->createSin(operand);
        });
    }

    Node* cos(Node* operand) {
        std::string key = keyForUnary("cos", operand);
        return getOrCreate(key, [this, operand]() {
            return allocator->createCos(operand);
        });
    }

    Node* ln(Node* operand) {
        std::string key = keyForUnary("ln", operand);
        return getOrCreate(key, [this, operand]() {
            return allocator->createLn(operand);
        });
    }

    Node* log(Node* base, Node* operand) {
        // For log, we build a key manually.
        std::string key = "log(" + base->toString() + "," + operand->toString() + ")";
        return getOrCreate(key, [this, base, operand]() {
            return allocator->createLog(base, operand);
        });
    }

    // Equality & Function Nodes with caching
    Node* eq(Node* left, Node* right) {
        std::string key = keyForBinary("eq", left, right);
        return getOrCreate(key, [this, left, right]() {
            return allocator->createEquality(left, right);
        });
    }

    Node* func(const std::string &name, int expectedArgCount, const std::vector<Node*>& args,
               FunctionCallback callback) {
        std::string key = keyForFunction(name, expectedArgCount, args);
        return getOrCreate(key, [this, name, expectedArgCount, args, callback]() {
            return allocator->createFunction(name, expectedArgCount, args, callback);
        });
    }
};

} // namespace Expression

#endif // NODE_FACTORY_H
