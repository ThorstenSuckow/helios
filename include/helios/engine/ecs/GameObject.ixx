/**
 * @file GameObject.ixx
 * @brief Base class for interactive game entities with transform capabilities.
 */
module;

#include <cassert>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>


export module helios.engine.ecs.GameObject;

import helios.util.Guid;
import helios.engine.ecs.Component;
import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.Updatable;

export namespace helios::engine::ecs {

    /**
     * @brief Container for components that represents an entity in the game world.
     *
     * @details A GameObject is the fundamental entity type in helios. It serves as a
     * container for Component instances that define its behavior and data. Each GameObject
     * has a unique Guid for identification and lookup within the GameWorld.
     *
     * ## Component Management
     *
     * Components are added via `add<T>()` and retrieved via `get<T>()`. The GameObject
     * uses a type-indexed map (`std::unordered_map<std::type_index, Component*>`) for
     * **O(1) amortized** component lookup by type.
     *
     * ```cpp
     * auto entity = std::make_unique<GameObject>();
     * entity->add<SceneNodeComponent>(sceneNode);
     * entity->add<Move2DComponent>();
     *
     * auto* move = entity->get<Move2DComponent>();  // O(1) lookup
     * ```
     *
     * ## Active State
     *
     * Each GameObject has an `isActive()` flag that controls participation in the game loop:
     *
     * | State | Behavior |
     * |-------|----------|
     * | `isActive() == true` | Processed by systems, rendered, collides |
     * | `isActive() == false` | Skipped by systems, exists but dormant |
     *
     * When the active state changes, `onActivate()` or `onDeactivate()` is called on all
     * attached components.
     *
     * ## Lifecycle Callbacks
     *
     * Components receive lifecycle notifications:
     * - `onAttach()` — Called when a component is added to the GameObject
     * - `onActivate()` / `onDeactivate()` — Called when active state changes
     * - `onAcquire()` / `onRelease()` — Called when acquired from/released to a pool
     *
     * ## Ownership
     *
     * GameObjects are typically owned by a GameWorld via `std::unique_ptr`. The GameWorld
     * provides lookup by Guid with amortized O(1) complexity.
     *
     * @see Component
     * @see GameWorld
     * @see GameObjectFilter
     */
    class GameObject {

    protected:

        /**
         * @brief Unique identifier for this GameObject.
         *
         * @details Generated during construction and remains constant for the lifetime
         * of the object. Used for lookups in GameWorld and command targeting.
         */
        helios::util::Guid guid_;

        /**
         * @brief List of components attached to this GameObject.
         *
         * @details Components are owned by the GameObject via std::unique_ptr.
         */
        std::vector<std::unique_ptr<Component>> components_;

        /**
         * @brief Type-indexed map for O(1) amortized component lookups.
         *
         * @details Maps `std::type_index` to raw component pointers for efficient
         * retrieval by type. The map is updated when components are added.
         * Worst-case is O(n) due to hash collisions, but typical lookups are O(1).
         */
        std::unordered_map<std::type_index, helios::engine::ecs::Component*> componentIndex_;

        /**
         * @brief Cached list of components that implement the Updatable interface.
         *
         * @details This list is maintained for efficient iteration during the update phase.
         * The pointers are non-owning references to elements within components_.
         */
        std::vector<Updatable*> updatables_;

        /**
         * @brief Active state flag for this GameObject.
         *
         * @details Inactive GameObjects are skipped during iteration and updates.
         * When the state changes, onActivate() or onDeactivate() is called on all components.
         */
        bool isActive_ = true;


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
         * The component is created, attached to this GameObject (onAttach is called),
         * and if it implements Updatable, it is added to the update list.
         * Normalizes T and uses the type-indexed componentIndex_ for O(1) lookup complexity.
         *
         * @tparam T The type of component to add. Must derive from Component.
         * @tparam Args Argument types for the component's constructor.
         * @param args Arguments forwarded to the component's constructor.
         *
         * @return Reference to the newly created component.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {
            using U = std::remove_cvref_t<T>;

            assert(!has<U>() && "Cannot add, Component is already existing.");

            auto component_ptr = std::make_unique<U>(std::forward<Args>(args)...);
            U* raw_component_ptr = component_ptr.get();

            if constexpr (std::derived_from<U, helios::engine::ecs::Updatable>) {
                updatables_.push_back(raw_component_ptr);
            }
            components_.push_back(std::move(component_ptr));
            componentIndex_[typeid(U)] = raw_component_ptr;

            // make sure component is registered before onAttach is called,
            // in case onAttach implementations query the gameObject for **this** component
            // Use raw_component_ptr instead of component_ptr since ownership was moved above
            finalizeAttach(raw_component_ptr);

            return *raw_component_ptr;
        }

        /**
         * @brief Adds a pre-constructed component if no component of the same type exists.
         *
         * @details If a component of the same runtime type already exists, returns the
         * existing component and does not take ownership of the passed pointer.
         * Otherwise, takes ownership and attaches the component.
         *
         * Uses the type-indexed componentIndex_ for O(1) amortized lookup complexity.
         *
         * @param component Unique pointer to the component to add. Ownership is transferred
         *                  only if no component of the same type exists.
         *
         * @return Pointer to the existing or newly added component, or nullptr if
         *         the input was nullptr.
         */
        helios::engine::ecs::Component* getOrAdd(
            std::unique_ptr<helios::engine::ecs::Component> component) {

            assert(component != nullptr && "Unexpected nullptr for component.");

            if (!component) {
                return nullptr;
            }

            auto* raw_component_ptr = component.get();
            const std::type_index idx = typeid(*raw_component_ptr);

            if (has(typeid(*raw_component_ptr))) {
                return componentIndex_[idx];
            }

            components_.push_back(std::move(component));
            componentIndex_[idx] = raw_component_ptr;

            // make sure component is registered before onAttach is called,
            // in case onAttach implementations query the gameObject for **this** component
            // Use raw_component_ptr instead of component_ptr since ownership was moved above
            finalizeAttach(raw_component_ptr);

            return raw_component_ptr;
        }


        /**
         * @brief Retrieves an existing component of type T, or creates and adds it if not present.
         *
        * Normalizes T and uses the type-indexed componentIndex_ for O(1) lookup complexity. This is a convenience
        * method that combines get() and add(). If the component already exists, the arguments are ignored.
         *
         * @tparam T The type of component to retrieve or add. Must derive from Component.
         * @tparam Args Argument types for the component's constructor.
         * @param args Arguments forwarded to the component's constructor if it needs to be created.
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
         * Normalizes T and uses the type-indexed componentIndex_ for O(1) lookup complexity.
         *
         * @tparam T The type of component to retrieve.
         *
         * @return Pointer to the component if found, nullptr otherwise.
         */
        template<typename T>
        std::remove_cvref_t<T>* get() const {
            // remove ref (if provided) and return type refered to by T
            using U = std::remove_cvref_t<T>;
            auto it = componentIndex_.find(typeid(U));

            if (it == componentIndex_.end()) {
                return nullptr;
            }

            return static_cast<U*>(it->second);
        }

        /**
         * @brief Checks if a component with the given type index exists.
         *
         * @details Uses the type-indexed componentIndex_ for O(1) amortized lookup.
         *
         * @param idx The type_index of the component type to check for.
         *
         * @return True if the component exists, false otherwise.
         */
        [[nodiscard]] bool has(const std::type_index idx) const noexcept {
            return componentIndex_.contains(idx);
        }


        /**
         * @brief Checks if the GameObject has a component of type T.
         *
         * Normalizes T and uses the type-indexed componentIndex_ for O(1) lookup complexity.
         *
         * @tparam T The type of component to check for.
         *
         * @return true if the component exists, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            using U = std::remove_cvref_t<T>;
            return componentIndex_.contains(typeid(U));
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
         * @brief Updates all updatable components attached to this GameObject.
         *
         * @param updateContext Context containing frame delta time and other update data.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext) {
            for (auto* updatable: updatables_) {
                updatable->update(updateContext);
            }
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
         * @brief Returns a reference to all attached components.
         *
         * @details Provides direct access to the component container. Useful for
         * iteration over all components (e.g., during cloning).
         *
         * @return Reference to the vector of component unique_ptrs.
         *
         * @warning Modifying the vector directly bypasses lifecycle callbacks.
         *          Use add() and remove() methods for proper component management.
         */
        [[nodiscard]] std::vector<std::unique_ptr<helios::engine::ecs::Component>>& components() noexcept {
            return components_;
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
                    it->onActivate();
                }
            } else {
                for (auto& it: components_) {
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
            for (auto [idx, component] : componentIndex_) {
                component->onAcquire();
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
            for (auto [idx, component] : componentIndex_) {
                component->onRelease();
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
            for (auto [idx, component] : componentIndex_) {
                if (component->isAttached()) {
                    return;
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


} // namespace helios::engine::game
