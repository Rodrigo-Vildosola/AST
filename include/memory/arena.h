#ifndef ARENA_H
#define ARENA_H

#include "_pch.h"
#include "expression/node.h"

namespace Expression {

/**
 * @brief Arena is a "full arena" allocator for Node objects.
 * It logs memory usage and tracks allocations. 
 * ALL node creation in your AST must go through this arena's `make()` method.
 */
class Arena {
private:
    std::vector<Node*> allocatedNodes;  ///< Tracks all allocated nodes
    size_t nodeCount = 0;              ///< How many nodes allocated
    size_t totalBytes = 0;             ///< Approx total size of allocated nodes

public:
    Arena() = default;
    
    // Prevent copying (ensures unique ownership)
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    /**
     * @brief Destructor automatically deletes all stored nodes.
     * Logs final stats if desired.
     */
    ~Arena() {
        // Optional logging
        std::cerr << "[Arena] Destroying: " 
                  << nodeCount << " nodes, ~" << totalBytes << " bytes.\n";

        // Delete all nodes
        for (Node* node : allocatedNodes) {
            delete node;
        }
    }

    /**
     * @brief Allocate and store a new Node, logging usage stats.
     * 
     * @tparam T The concrete Node type
     * @tparam Args Constructor parameter types
     * @param args Parameters to forward to T's constructor
     * @return T* A pointer to the newly allocated Node
     */
    template<typename T, typename... Args>
    T* make(Args&&... args) {
        static_assert(std::is_base_of<Node, T>::value, 
                      "T must inherit from Node");

        // Create the node
        T* node = new T(std::forward<Args>(args)...);
        allocatedNodes.push_back(node);

        // Log basic usage stats
        nodeCount++;
        // This is approximate. We only account for `sizeof(T)`. 
        // If T has internal allocations, we won't see those.
        totalBytes += sizeof(T);

        return node;
    }

    /**
     * @brief Get how many nodes have been allocated
     */
    size_t getNodeCount() const {
        return nodeCount;
    }

    /**
     * @brief Get approximate total bytes allocated in this arena.
     */
    size_t getTotalBytes() const {
        return totalBytes;
    }

    /**
     * @brief Optional debug method for printing usage stats.
     */
    void printStats() const {
        std::cout << "[Arena] Currently allocated: " 
                  << nodeCount << " nodes, ~" << totalBytes << " bytes.\n";
    }
};

}  // namespace Expression

#endif  // EXPR_ARENA_H
