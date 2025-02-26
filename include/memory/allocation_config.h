#ifndef ALLOCATION_CONFIG_H
#define ALLOCATION_CONFIG_H

namespace Expression {

enum class DeletionPolicy {
    Default, // Nodes delete their children.
    Arena    // Arena manages deletion.
};

class AllocationConfig {
public:
    static DeletionPolicy getPolicy() {
        return policy;
    }
    static void setPolicy(DeletionPolicy p) {
        policy = p;
    }
private:
    static DeletionPolicy policy;
};

} // namespace Expression

#endif
