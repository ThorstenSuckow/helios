/**
 * @file GameObject.ixx
 * @brief Base class for interactive game entities with transform capabilities.
 */
module;

#include <memory>
#include <vector>


export module helios.engine.game.GameObject;

import helios.util.Guid;
import helios.engine.game.Component;
import helios.engine.game.UpdateContext;
import helios.engine.game.Updatable;

export namespace helios::engine::game {

    /**
     * @brief Base class for interactive game entities using a component-based architecture.
     *
     * @details A GameObject is a container for Component instances that define its behavior.
     * Each GameObject has a unique Guid for identification in the GameWorld and supports
     * per-frame updates for components implementing the Updatable interface.
     *
     * GameObjects are typically owned by a GameWorld and should be created using
     * std::make_unique before being added to the world.
     *
     * Example usage:
     * ```cpp
     * auto entity = std::make_unique<helios::engine::game::GameObject>();
     * entity->add<SceneNodeComponent>(sceneNode);
     * entity->add<Move2DComponent>();
     *
     * auto* ptr = world.addGameObject(std::move(entity));
     * ```
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
         * @brief Cached list of components that implement the Updatable interface.
         *
         * @details This list is maintained for efficient iteration during the update phase.
         * The pointers are non-owning references to elements within components_.
         */
        std::vector<Updatable*> updatables_;

    public:
        GameObject() : guid_(helios::util::Guid::generate()) {};

        virtual ~GameObject() = default;

        /**
         * @brief Adds a new component of type T to the GameObject.
         *
         * @tparam T The type of component to add. Must derive from Component.
         * @tparam Args Argument types for the component's constructor.
         * @param args Arguments forwarded to the component's constructor.
         * @return Reference to the newly created component.
         *
         * @details The component is created, attached to this GameObject (onAttach is called),
         * and if it implements Updatable, it is added to the update list.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {

            auto component_ptr = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw_component_ptr = component_ptr.get();

            component_ptr->onAttach(this);

            if constexpr (std::derived_from<T, helios::engine::game::Updatable>) {
                updatables_.push_back(raw_component_ptr);
            }

            components_.push_back(std::move(component_ptr));

            return *raw_component_ptr;
        }

        /**
         * @brief Retrieves a component of type T.
         *
         * @tparam T The type of component to retrieve.
         * @return Pointer to the component if found, nullptr otherwise.
         *
         * @details Performs a linear search through the components.
         */
        template<typename T>
        T* get() const {
            for (const auto& component : components_) {
                if (auto* comp = dynamic_cast<T*>(component.get())) {
                    return comp;
                }
            }

            return nullptr;
        }


        /**
         * @brief Checks if the GameObject has a component of type T.
         *
         * @tparam T The type of component to check for.
         * @return true if the component exists, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            return get<T>() != nullptr;
        }

        /**
         * @brief Updates all updatable components attached to this GameObject.
         *
         * @param updateContext Context containing frame delta time and other update data.
         */
        void update(helios::engine::game::UpdateContext& updateContext) {
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
    };


} // namespace helios::engine::game
