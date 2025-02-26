#ifndef ARENA_H
#define ARENA_H

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstddef>
#include <new>
#include <iostream>
#include "expression/node.h"

namespace Expression {

class Arena {
private:
    // Internal structure for a memory block.
    struct Block {
        char* memory;
        size_t size;
        size_t offset;

        Block(size_t sz) : size(sz), offset(0) {
            // Allocate the block with proper alignment.
            memory = new char[size];
        }
        ~Block() {
            delete[] memory;
        }
    };

    std::vector<Block*> blocks;  ///< Vector of allocated blocks
    size_t defaultBlockSize;       ///< Default size for new blocks
    size_t nodeCount = 0;          ///< Count of allocated nodes

public:
    // Initialize arena with a default block size.
    explicit Arena(size_t blockSize = 4096)
        : defaultBlockSize(blockSize) {
        blocks.push_back(new Block(defaultBlockSize));
    }

    // Disable copying
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    ~Arena() {
        std::cerr << "[Arena] Destroying: " 
                  << nodeCount << " nodes allocated.\n";
        for (Block* block : blocks) {
            delete block;
        }
    }

    /**
     * @brief Allocate a new Node of type T using placement new.
     * 
     * @tparam T Must be derived from Node.
     * @tparam Args Constructor arguments for T.
     * @return T* Pointer to the new Node.
     */
    template<typename T, typename... Args>
    T* make(Args&&... args) {
        static_assert(std::is_base_of<Node, T>::value, "T must inherit from Node");

        size_t spaceNeeded = sizeof(T);
        // Get the current block.
        Block* currentBlock = blocks.back();

        // Check if there's enough space, else allocate a new block.
        if (currentBlock->offset + spaceNeeded > currentBlock->size) {
            // Determine new block size (at least defaultBlockSize or larger if needed)
            size_t newBlockSize = std::max(defaultBlockSize, spaceNeeded);
            currentBlock = new Block(newBlockSize);
            blocks.push_back(currentBlock);
        }

        // Allocate memory from the block.
        void* mem = currentBlock->memory + currentBlock->offset;
        currentBlock->offset += spaceNeeded;

        // Construct the node using placement new.
        T* node = new (mem) T(std::forward<Args>(args)...);
        ++nodeCount;
        return node;
    }

    // Optional debug method for printing usage stats.
    void printStats() const {
        size_t totalBytes = 0;
        for (const Block* block : blocks)
            totalBytes += block->size;
        std::cout << "[Arena] Blocks: " << blocks.size() 
                  << ", Total Memory: ~" << totalBytes 
                  << " bytes, Nodes: " << nodeCount << std::endl;
    }
};

}  // namespace Expression

#endif  // ARENA_H
