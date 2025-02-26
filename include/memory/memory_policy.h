// In a common header, e.g., "memory/memory_policy.h":
#pragma once

namespace Expression {
    // When true, node destructors will delete their children.
    // When false, they won't, because the allocator will take care of it.
    extern bool g_DeleteChildren;
}
