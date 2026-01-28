/**
 * @file GameObject.ixx
 * @brief Base class for interactive game entities.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <type_traits>
#include <typeindex>
#include <vector>


export module helios.engine.ecs.GameObject;

import helios.util.Guid;
import helios.engine.ecs.Component;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.core.data.ComponentTypeId;

export namespace helios::engine::ecs {

    /**
     * @brief Base class for interactive game entities with component-based composition.
     *
     * @details GameObject serves as the fundamental building block for all interactive entities
     * in the helios engine. It provides a type-indexed component storage system that enables
     * O(1) component access and modification.
     *
     * **Component Storage Architecture:**
     * Components are stored in a vector indexed by `ComponentTypeId`. Each component type
     * receives a unique compile-time ID, which determines its position in the storage vector.
     * This approach trades memory (sparse vector with nullptr slots) for speed (direct indexing
     * without hashing).
     *
     * **Lifecycle Management:**
     * The GameObject manages component lifecycles through callbacks:
     * - `onAttach()` - Called when a component is added.
     * - `onActivate()` / `onDeactivate()` - Called when the GameObject's active state changes.
     * - `onAcquire()` / `onRelease()` - Called during pool-based lifecycle management.
     *
     * **Storage Model:**
     * Components are stored in a `std::vector<std::unique_ptr<Component>>` where the index
     * corresponds to the component's `ComponentTypeId::value()`. The vector is resized on demand
     * when new component types are added. Slots for component types that are not attached to
     * this GameObject remain nullptr.
     *
     * **Performance Characteristics:**
     * - `get<T>()`: O(1) - Direct array access via type ID.
     * - `has<T>()`: O(1) - Bounds check + nullptr check.
     * - `add<T>()`: O(1) amortized - May trigger resize if type ID exceeds current capacity.
     * - `components()`: O(n) on first call after modification (cache rebuild), O(1) thereafter.
     *
     * @see Component
     * @see GameWorld
     * @see GameObjectPool
     */
    class GameObject {

    protected:

        using ComponentTypeId = helios::engine::core::data::ComponentTypeId;

        /**
         * @brief Unique identifier for this GameObject.
         *
         * @details Generated during construction and remains constant for the lifetime
         * of the object. Used for lookups in GameWorld and command targeting.
         */
        helios::util::Guid guid_;

        /**
         * @brief Type-indexed storage for components attached to this GameObject.
         *
         * @details Components are stored at indices corresponding to their `ComponentTypeId::value()`.
         * The vector is resized dynamically when component types with higher IDs are added.
         * Slots for component types not attached to this GameObject contain nullptr.
         *
         * This storage model provides O(1) access and modification at the cost of potential
         * sparse memory usage (nullptr entries for unused component type IDs).
         *
         * @note The vector may contain nullptr entries. Use `components()` for iteration
         *       over non-null components, which returns a cached, filtered view.
         */
        std::vector<std::unique_ptr<Component>> components_;

        /**
         * @brief Active state flag for this GameObject.
         *
         * @details Inactive GameObjects are skipped during iteration and updates.
         * When the state changes, onActivate() or onDeactivate() is called on all components.
         */
        bool isActive_ = true;

        /**
         * @brief Flag indicating whether the component cache needs to be rebuilt.
         *
         * @details Set to true when components are added or removed. The cache is
         * lazily rebuilt on the next call to `components()`.
         */
        mutable bool needsUpdate_ = true;

        /**
         * @brief Cached view of non-null component pointers for mutable access.
         *
         * @details Rebuilt from `components_` when `needsUpdate_` is true.
         * Provides filtered access excluding nullptr entries.
         */
        mutable std::vector<helios::engine::ecs::Component*> componentView_;

        /**
         * @brief Cached view of non-null component pointers for const access.
         *
         * @details Rebuilt from `components_` when `needsUpdate_` is true.
         * Provides filtered access excluding nullptr entries.
         */
        mutable std::vector<const helios::engine::ecs::Component*> componentConstView_;

        /**
         * @brief Invalidates the component cache, forcing a rebuild on next access.
         *
         * @details Called automatically when components are added or removed.
         */
        void invalidateCache() noexcept { needsUpdate_ = true; }

        /**
         * @brief Rebuilds the component cache if invalidated.
         *
         * @details Iterates through `components_` and populates `componentView_` and
         * `componentConstView_` with non-null entries. This operation is O(n) where
         * n is the size of the components vector.
         */
        void updateCache() const {

            if (!needsUpdate_) {
                return;
            }
            componentView_.clear();
            componentConstView_.clear();
            componentView_.reserve(components_.size());
            componentConstView_.reserve(components_.size());

            for (const auto& component : components_) {

                if (component == nullptr) {
                    continue;
                }
                componentView_.push_back(component.get());
                componentConstView_.push_back(component.get());
            }

           needsUpdate_ = false;
        }


    public:
        /**
         * @brief Constructs a new GameObject with a unique Guid.
         *
         * @details The Guid is automatically generated and remains constant for the
         * lifetime of this object.
         */
        GameObject() : guid_(helios::util::Guid::generate()) {};

        /**
         * @brief Constructs a new GameObject with a specified active state.
         *
         * @param isActive Initial active state. If false, the object starts inactive.
         */
        explicit GameObject(bool isActive) : isActive_(isActive), guid_(helios::util::Guid::generate()) {};

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~GameObject() = default;

        /**
         * @brief Adds a new component of type T to the GameObject.
         *
         * @details The component is created in-place, stored at the index corresponding to
         * its `ComponentTypeId`, and attached to this GameObject (`onAttach()` is called).
         * If the type ID exceeds the current vector capacity, the vector is resized.
         *
         * **Complexity:** O(1) amortized. Resize operations are O(n) but occur infrequently.
         *
         * @tparam T The type of component to add. Must derive from Component.
         * @tparam Args Argument types for the component's constructor.
         *
         * @param args Arguments forwarded to the component's constructor.
         *
         * @return Reference to the newly created component.
         *
         * @pre No component of type T is already attached (asserts in debug mode).
         *
         * @note The component is stored directly at `components_[ComponentTypeId::id<T>().value()]`.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {
            using U = std::remove_cvref_t<T>;

            assert(!has<U>() && "Cannot add, Component is already existing.");

            auto component_ptr = std::make_unique<U>(std::forward<Args>(args)...);
            U* raw_component_ptr = component_ptr.get();

            const auto componentTypeId = ComponentTypeId::id<U>();
            if (componentTypeId.value() >= components_.size()) {
                components_.resize(componentTypeId.value() + 1);
            }

            components_[componentTypeId.value()] = std::move(component_ptr);

            // make sure component is registered before onAttach is called,
            // in case onAttach implementations query the gameObject for **this** component
            // Use raw_component_ptr instead of component_ptr since ownership was moved above
            finalizeAttach(raw_component_ptr);
            invalidateCache();

            return *raw_component_ptr;
        }

        /**
         * @brief Adds a pre-constructed component if no component of the same type exists.
         *
         * @details If a component with the same `ComponentTypeId` already exists, returns
         * the existing component without taking ownership of the passed pointer.
         * Otherwise, stores the component at its type-indexed position and takes ownership.
         *
         * **Complexity:** O(1) amortized. May trigger resize if type ID exceeds capacity.
         *
         * @param component Unique pointer to the component to add. Ownership is transferred
         *                  only if no component of the same type exists.
         *
         * @return Pointer to the existing or newly added component, or nullptr if
         *         the input was nullptr.
         *
         * @note Uses `component->typeId()` for runtime type identification.
         */
         helios::engine::ecs::Component* getOrAdd(
            std::unique_ptr<helios::engine::ecs::Component> component) {

            assert(component != nullptr && "Unexpected nullptr for component.");

            if (!component) {
                return nullptr;
            }

            const auto componentTypeId = component.get()->typeId();

            if (has(componentTypeId)) {
                return components_[componentTypeId.value()].get();
            }

            if (componentTypeId.value() >= components_.size()) {
                components_.resize(componentTypeId.value() + 1);
            }

            auto* raw_component_ptr = component.get();
            components_[componentTypeId.value()] = std::move(component);

            // make sure component is registered before onAttach is called,
            // in case onAttach implementations query the gameObject for **this** component
            // Use raw_component_ptr instead of component_ptr since ownership was moved above
            finalizeAttach(raw_component_ptr);
            invalidateCache();
            return raw_component_ptr;
        }


        /**
         * @brief Retrieves an existing component of type T, or creates and adds it if not present.
         *
         * @details Convenience method that combines `get<T>()` and `add<T>()`.
         * If the component already exists, the constructor arguments are ignored.
         *
         * **Complexity:** O(1) amortized.
         *
         * @tparam T The type of component to retrieve or add. Must derive from Component.
         * @tparam Args Argument types for the component's constructor.
         *
         * @param args Arguments forwarded to the component's constructor if creation is needed.
         *
         * @return Reference to the existing or newly created component.
         */
        template<typename T, typename... Args>
        std::remove_cvref_t<T>& getOrAdd(Args&&... args) {

            using U = std::remove_cvref_t<T>;
            if (auto* cmp = get<U>()) {
                return *cmp;
            }

            return add<U>(std::forward<Args>(args)...);
        }

        /**
         * @brief Retrieves a component of type T.
         *
         * @details Performs a direct array access using the component's type ID.
         * If the type ID is out of bounds or the slot is nullptr, returns nullptr.
         *
         * **Complexity:** O(1) - Direct index access without hashing.
         *
         * @tparam T The type of component to retrieve. cv-qualifiers are stripped.
         *
         * @return Pointer to the component if found, nullptr otherwise.
         */
        template<typename T>
        [[nodiscard]] std::remove_cvref_t<T>* get() const {
            // remove ref (if provided) and return type refered to by T
            using U = std::remove_cvref_t<T>;

            const auto componentTypeId = ComponentTypeId::id<U>();

            if (componentTypeId.value() >= components_.size()) {
                return nullptr;
            }

            auto* cmp = components_[componentTypeId.value()].get();

            if (!cmp) {
                return nullptr;
            }

            return static_cast<U*>(cmp);
        }

        /**
         * @brief Retrieves a component by its runtime ComponentTypeId.
         *
         * @details Performs a direct array access using the provided type ID value.
         * Useful when the component type is not known at compile time.
         *
         * **Complexity:** O(1) - Direct index access.
         *
         * @param componentTypeId The runtime type ID of the component.
         *
         * @return Pointer to the component if found, nullptr otherwise.
         */
        [[nodiscard]] helios::engine::ecs::Component* get(const ComponentTypeId componentTypeId) const noexcept {

            if (!has(componentTypeId)) {
                return nullptr;
            }

            return components_[componentTypeId.value()].get();
        }

        /**
         * @brief Checks if a component with the given type ID exists.
         *
         * @details Performs bounds checking and nullptr verification.
         *
         * **Complexity:** O(1) - Bounds check + nullptr comparison.
         *
         * @param componentTypeId The runtime type ID of the component to check.
         *
         * @return True if a component with this type ID exists, false otherwise.
         */
        [[nodiscard]] bool has(const ComponentTypeId componentTypeId) const noexcept {

            if (componentTypeId.value() >= components_.size()) {
                return false;
            }
            return components_[componentTypeId.value()] != nullptr;
        }


        /**
         * @brief Checks if the GameObject has a component of type T.
         *
         * @details Delegates to `has(ComponentTypeId)` using the compile-time type ID.
         *
         * **Complexity:** O(1) - Direct index bounds check + nullptr comparison.
         *
         * @tparam T The type of component to check for. cv-qualifiers are stripped.
         *
         * @return True if the component exists, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            using U = std::remove_cvref_t<T>;
            return has(ComponentTypeId::id<U>());
        }

        /**
        * @brief Checks if the GameObject has a component of type T and returns true if
        * this component is enabled.
        *
        * @tparam T The type of component to check for existence and if it's enabled.
        *
        * @return true if the component exists and is enabled, false otherwise.
        */
        template<typename T>
        [[nodiscard]] bool hasEnabledComponent() const {
            using U = std::remove_cvref_t<T>;
            U* cmp = get<T>();

            if (!cmp) {
                return false;
            }

            return cmp->isEnabled();
        }

        /**
         * @brief Checks if the GameObject has a component of type T and returns true if
         * this component is disabled.
         *
         * @tparam T The type of component to check for existence and if it's disabled.
         *
         * @return true if the component exists and is disabled, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool hasDisabledComponent() const {
            using U = std::remove_cvref_t<T>;
            U* cmp = get<T>();

            if (!cmp) {
                return false;
            }

            return cmp->isDisabled();
        }

        /**
         * @brief Returns the unique identifier for this GameObject.
         *
         * @return Const reference to the Guid, valid for the lifetime of this object.
         */
        [[nodiscard]] const helios::util::Guid& guid() const noexcept {
            return guid_;
        };

        /**
         * @brief Checks if this GameObject is currently active.
         *
         * @return True if the GameObject is active, false otherwise.
         *
         * @note Inactive GameObjects are typically skipped during updates and rendering.
         */
        [[nodiscard]] bool isActive() const noexcept {
            return isActive_;
        }

        /**
         * @brief Returns a span over all attached (non-null) components.
         *
         * @details Provides access to a cached, filtered view of components. The cache is
         * lazily rebuilt when components are added or removed.
         *
         * **Complexity:** O(1) if cache is valid, O(n) on first call after modification.
         *
         * @return Span of non-null component pointers for mutable access.
         *
         * @warning Modifying the underlying vector bypasses lifecycle callbacks.
         *          Use `add()` for proper component management.
         */
        [[nodiscard]] std::span<helios::engine::ecs::Component* const> components() noexcept {
            updateCache();
            return {componentView_.data(), componentView_.size()};
        }

        /**
         * @brief Returns a const span over all attached (non-null) components.
         *
         * @details Provides read-only access to a cached, filtered view of components.
         *
         * **Complexity:** O(1) if cache is valid, O(n) on first call after modification.
         *
         * @return Span of non-null component pointers for const access.
         */
        [[nodiscard]] std::span<const helios::engine::ecs::Component* const> components() const noexcept {
            updateCache();
            return {componentConstView_.data(), componentConstView_.size()};
        }

        /**
         * @brief Sets the active state of this GameObject.
         *
         * @details When the active state changes, this method notifies all attached
         * components by calling onActivate() or onDeactivate() accordingly.
         * If the new state equals the current state, no action is taken. (@see add())
         *
         * @param active The new active state.
         */
        void setActive(bool active) noexcept {
            if (isActive_ == active) {
                return;
            }

            isActive_ = active;

            if (isActive_) {
                for (auto& it: components_) {
                    if (!it) {
                        continue;
                    }
                    it->onActivate();
                }
            } else {
                for (auto& it: components_) {
                    if (!it) {
                        continue;
                    }
                    it->onDeactivate();
                }
            }


        }

        /**
         * @brief Called when this GameObject is acquired from a pool.
         *
         * @details Notifies all attached components by calling their `onAcquire()` method.
         * Used by the pool system to reset or initialize component state before reuse.
         *
         * @see GameObjectPool
         * @see Component::onAcquire
         */
        void onAcquire() {
            for (const auto& component : components_) {
                if (component != nullptr) {
                    component->onAcquire();
                }
            }
        }

        /**
         * @brief Called when this GameObject is released back to a pool.
         *
         * @details Notifies all attached components by calling their `onRelease()` method.
         * Used by the pool system to clean up component state before pooling.
         *
         * @see GameObjectPool
         * @see Component::onRelease
         */
        void onRelease() {
            for (const auto& component : components_) {
                if (component != nullptr) {
                    component->onRelease();
                }
            }
        }

        /**
         * @brief Finalizes attachment of components by calling lifecycle callbacks.
         *
         * @details If a specific component is passed, only that component is finalized.
         * Otherwise, all unattached components are finalized. Calls `onAttach()` followed
         * by `onActivate()` or `onDeactivate()` based on the GameObject's active state.
         *
         * @param component Optional pointer to a specific component to finalize.
         *                  If nullptr, all components are processed.
         */
        void finalizeAttach(helios::engine::ecs::Component* component = nullptr) {

            // call attach on all components
            if (component != nullptr) {
                if (component->isAttached()) {
                    return;
                }
                component->onAttach(this);

                if (isActive_) {
                    component->onActivate();
                } else {
                    component->onDeactivate();
                }

                return;
            }

            // call attach on all components
            for (const auto& component : components_) {
                if (component == nullptr) {
                    continue;
                }
                if (component->isAttached()) {
                    continue;
                }
                component->onAttach(this);

                if (isActive_) {
                    component->onActivate();
                } else {
                    component->onDeactivate();
                }
            }

        }

    };


} // namespace helios::engine::modules
