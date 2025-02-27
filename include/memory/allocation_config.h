#ifndef ALLOCATION_CONFIG_H
#define ALLOCATION_CONFIG_H

namespace Expression {

enum class AllocatorPolicy {
    Default, // Nodes delete their children.
    Arena    // Arena manages deletion.
};

class AllocationConfig {
public:
    static AllocatorPolicy getPolicy() {
        return policy;
    }
    static void setPolicy(AllocatorPolicy p) {
        policy = p;
    }
private:
    static AllocatorPolicy policy;
};

} // namespace Expression

#endif
