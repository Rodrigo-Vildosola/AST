#ifndef EXPR_ARENA_H
#define EXPR_ARENA_H

#include "expression/node.h"  // Base class for expressions

namespace Expression {

class ExprArena {
private:
    std::vector<Node*> allocatedNodes;  // Tracks all allocated nodes

public:
    ExprArena() = default;
    
    // Prevent copying (ensures unique ownership)
    ExprArena(const ExprArena&) = delete;
    ExprArena& operator=(const ExprArena&) = delete;

    // Destructor: Automatically deletes all stored nodes
    ~ExprArena() {
        for (Node* node : allocatedNodes) {
            delete node;
        }
    }

    // Allocate and store a new Node
    template<typename T, typename... Args>
    T* make(Args&&... args) {
        T* node = new T(std::forward<Args>(args)...);
        allocatedNodes.push_back(node);
        return node;
    }
};

}  // namespace Expression

#endif  // EXPR_ARENA_H
