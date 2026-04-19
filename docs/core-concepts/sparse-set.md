# Sparse Set

The **SparseSet** is a generic data structure providing O(1) insertion, lookup, and removal operations. It is a fundamental building block for the helios ECS (Entity Component System), used internally for efficient component storage.

## SparseSetBase

`SparseSetBase` is an abstract base class providing a type-erased interface for sparse sets. This enables polymorphic access to heterogeneous component pools:

```cpp
// EntityManager stores different component types uniformly
std::vector<std::unique_ptr<SparseSetBase>> components_;

// Remove component without knowing the concrete type
components_[typeId]->remove(entityId);
```

## Overview

A sparse set maintains two arrays:

- **Sparse array:** Maps entity indices to positions in the dense array
- **Dense array:** Stores actual data contiguously in memory

This design enables:
- **O(1) access** via direct indexing
- **O(1) removal** via swap-and-pop
- **Cache-friendly iteration** over dense storage

```
┌─────────────────────────────────────────────────────────────────────┐
│                      SPARSE SET ARCHITECTURE                        │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  SPARSE ARRAY (indexed by EntityId)                                 │
│  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐                        │
│  │  2  │  ×  │  0  │  ×  │  1  │  ×  │  ×  │  (dense index)         │
│  └──┬──┴─────┴──┬──┴─────┴──┬──┴─────┴─────┘                        │
│     │           │           │                                       │
│     │           │           └──────────────┐                        │
│     │           └────────────────┐         │                        │
│     └──────────────────────┐     │         │                        │
│                            ▼     ▼         ▼                        │
│  DENSE STORAGE (contiguous memory)                                  │
│  ┌─────────┬─────────┬─────────┐                                    │
│  │  T[0]   │  T[1]   │  T[2]   │                                    │
│  │ (id=2)  │ (id=4)  │ (id=0)  │                                    │
│  └─────────┴─────────┴─────────┘                                    │
│                                                                     │
│  DENSE-TO-SPARSE (reverse mapping for swap-and-pop)                 │
│  ┌─────┬─────┬─────┐                                                │
│  │  2  │  4  │  0  │  (EntityId)                                    │
│  └─────┴─────┴─────┘                                                │
│                                                                     │
│  × = Tombstone (empty slot)                                         │
└─────────────────────────────────────────────────────────────────────┘
```

## Complexity

| Operation | Time    | Space        |
|-----------|---------|--------------|
| emplace   | O(1)*   | O(max_id)    |
| insert    | O(1)*   | O(max_id)    |
| get       | O(1)    | -            |
| remove    | O(1)    | -            |

*Amortized due to potential sparse array resize.

## Usage

### Basic Operations

```cpp
import helios.engine.ecs.SparseSet;

using namespace helios::engine::ecs;

// Create a sparse set for transform components
SparseSet<TransformComponent> transforms;

// In-place construction with emplace (forwards arguments to T's constructor)
auto* transform = transforms.emplace(42, glm::vec3{0.0f}, glm::quat{}, glm::vec3{1.0f});

// Or insert a pre-constructed object (moved)
TransformComponent tc{glm::vec3{1.0f}, glm::quat{}, glm::vec3{1.0f}};
auto* inserted = transforms.insert(43, std::move(tc));

// Mutable access
if (auto* t = transforms.get(42)) {
    t->position = glm::vec3{10.0f, 0.0f, 0.0f};
}

// Const access (read-only)
const auto& constTransforms = transforms;
if (const auto* t = constTransforms.get(42)) {
    // t->position = ...;  // Error: const pointer
}

// Remove component
transforms.remove(42);
```

### Removal Interception with Traits

Components can intercept removal by implementing the `HasOnRemove` trait. If the `onRemove()` method returns `false`, removal is cancelled.

```cpp
import helios.engine.ecs.Traits;

// Component with removal interception
struct ResourceComponent {
    bool inUse = false;

    // Trait implementation: return false to cancel removal
    bool onRemove() {
        if (inUse) {
            return false;  // Cancel removal while in use
        }
        releaseResources();
        return true;  // Allow removal
    }

    void releaseResources() { /* ... */ }
};

// Verify trait is satisfied
static_assert(helios::engine::ecs::traits::HasOnRemove<ResourceComponent>);

// Usage
SparseSet<ResourceComponent> resources;
resources.emplace(42);

// Removal will call onRemove() at compile-time if trait is satisfied
resources.remove(42);  // Calls component.onRemove() before removal
```

## Swap-and-Pop Removal

When removing an element, the sparse set uses the **swap-and-pop** technique to maintain dense storage contiguity:

1. Move the last element to the position of the removed element
2. Update the sparse array entry for the moved element
3. Pop the last element from dense storage
4. Mark the removed slot as `Tombstone`

This avoids expensive array shifts and maintains O(1) removal.

```
Before removing entity 2 (at dense index 1):

         EntityId:  0     1     2     3
Sparse:           [ 0  |  ×  |  1  |  2  ]     (EntityId → Dense Index)
                    │         │     │
                    │    ┌────┘     │
                    │    │    ┌─────┘
                    ▼    ▼    ▼
Dense:            [ A  |  B  |  C  ]           (Contiguous storage)
                    0     1     2              (Dense Index)

DenseToSparse:    [ 0  |  2  |  3  ]           (Dense Index → EntityId)

× = Tombstone (empty slot)

─────────────────────────────────────────────────────

After removing entity 2 (swap C into position 1, pop):

         EntityId:  0     1     2     3
Sparse:           [ 0  |  ×  |  ×  |  1  ]     Entity 3 now at dense index 1
                    │               │
                    │    ┌──────────┘
                    ▼    ▼
Dense:            [ A  |  C  ]                 B removed, C moved
                    0     1

DenseToSparse:    [ 0  |  3  ]                 Position 1 now holds entity 3
```

## Memory Considerations

The sparse array grows to accommodate the maximum EntityId used. For applications with large, sparse entity ID ranges, this may result in significant memory usage. Consider:

- **Compact entity IDs:** Use entity recycling to keep IDs within a reasonable range
- **Paged sparse arrays:** For very large ID ranges, a paged implementation can reduce memory overhead

## Related Modules

- `helios.core.data.EntityId` - Entity identifier type
- `helios.core.data.EntityTombstone` - Sentinel value for empty slots
- `helios.core.data.EntityHandle` - Versioned entity reference

## See Also

- [EntityManager](ecs/entity-manager.md) - Uses SparseSet for component storage
- [GameObject](ecs/gameobject.md) - High-level entity wrapper
- [Component System](component-system.md) - ECS component architecture
- [Component Lifecycle](component-lifecycle.md) - Lifecycle hooks including onRemove



