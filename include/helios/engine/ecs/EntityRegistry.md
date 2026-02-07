# EntityRegistry

```cpp

module;

export module helios.engine.ecs.ComponentPool;

import helios.engine.ecs.EntityPool;

export namespace helios::engine::ecs {

    class ComponentPool {

    public:
        virtual ~ComponentPool() = default;

    };

    template <typename T>
    class TypedComponentPool : public ComponentPool {

        helios::engine::ecs::EntityPool<T> pool_;

    public:

        TypedComponentPool() = default;

        [[nodiscard]] helios::engine::ecs::EntityPool<T>& entityPool() {
            return pool_;
        }

    };


}

// Usage in EntityRegistry:

std::vector<std::unique_ptr<helios::engine::ecs::ComponentPool>> components_;

template<typename T>
helios::engine::ecs::EntityPool<T>& componentPool() {
    const auto typeId = helios::engine::core::data::ComponentTypeId::id<T>();

    if (typeId >= components_.size()) {
        components_.resize(typeId + 1);
    }

    if (!components_[typeId]) {
        components_[typeId] = std::make_unique<helios::engine::ecs::TypedComponentPool<T>>();
    }

    auto* pool = static_cast<helios::engine::ecs::TypedComponentPool<T>*>(components_[typeId].get());

    return pool->entityPool();
}
```

 - EntityRegistry organizes an EntityPool of (non-unique pointered) entites.
 - Additionally, the above mentioned datastructure for maintaining lists of components, which are plain structs, with no inheritance.
