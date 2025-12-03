/**
 * @file GameObject.ixx
 * @brief Base class for interactive game entities with transform capabilities.
 */
module;

export module helios.game.GameObject;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;

export namespace helios::game {

    /**
     * @brief Base class representing an interactive game entity in the game world.
     *
     * @details The `GameObject` provides a game-logic-level abstraction over a `SceneNode`.
     * It serves as the primary building block for game-specific entities like players or enemies,
     * designed to be subclassed to add further behavior. Each `GameObject` has a unique
     * `Guid` for easy identification and provides convenient methods for manipulating the
     * transform of its underlying `SceneNode`. It also acts as the target for `Command` execution.
     *
     * Example usage:
     * ```cpp
     * class Player : public helios::game::GameObject {
     * public:
     *     using GameObject::GameObject;
     *
     *     void jump() {
     *         translate({0.0f, 5.0f, 0.0f});
     *     }
     * };
     *
     * auto sceneNode = std::make_shared<helios::scene::SceneNode>();
     * auto player = std::make_unique<Player>(sceneNode.get());
     * player->translate({1.0f, 0.0f, 0.0f});
     * ```
     *
     * @warning The GameObject does NOT own the SceneNode. The caller must ensure the SceneNode
     *          outlives the GameObject. If the SceneNode is destroyed while the GameObject is
     *          still alive, all transform operations will result in undefined behavior.
     *
     * @todo Implement observer pattern or weak_ptr to detect SceneNode destruction and
     *       invalidate the transform pointer to prevent dangling pointer issues.
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
         * @brief Pointer to the SceneNode's transform component.
         *
         * @details This is a non-owning pointer. The GameObject does not manage the lifetime
         * of the SceneNode or its Transform. The pointer remains valid only as long as the
         * associated SceneNode exists.
         *
         * @warning This pointer can become dangling if the SceneNode is destroyed before
         *          this GameObject. Accessing a dangling pointer results in undefined behavior.
         *
         * @todo Implement notification mechanism to invalidate this pointer when the
         *       SceneNode is destroyed. Consider using observer pattern or std::weak_ptr
         *       to safely detect SceneNode destruction.
         */
        helios::scene::Transform* transform_;

    public:

        /**
         * @brief Constructs a GameObject associated with a SceneNode.
         *
         * @param sceneNode Pointer to the SceneNode this GameObject wraps. Must not be null.
         *
         * @pre sceneNode != nullptr
         *
         * @note The GameObject does NOT take ownership of the SceneNode. The caller is responsible
         *       for ensuring the SceneNode outlives this GameObject.
         * @note A unique Guid is automatically generated during construction.
         *
         * @warning Passing a null sceneNode will result in undefined behavior when accessing
         *          transform methods. In debug builds, this should trigger an assertion.
         */
        explicit GameObject(helios::scene::SceneNode* sceneNode) :
        transform_(&(sceneNode->localTransform())),
        guid_(helios::util::Guid::generate())
        {}

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~GameObject() = default;

        virtual void update(const float DELTA_TIME) = 0;

        /**
         * @brief Returns the unique identifier of this GameObject.
         *
         * @return Const reference to the GameObject's Guid.
         *
         * @note The Guid is generated during construction and never changes.
         */
        [[nodiscard]] const helios::util::Guid& guid() const noexcept {
            return guid_;
        }

        /**
         * @brief Applies a scaling transformation to the GameObject.
         *
         * @param scale The scale vector to apply (x, y, z components).
         *
         * @return Reference to this GameObject for method chaining.
         *
         * @note This modifies the underlying SceneNode's local transform.
         *
         * @warning If the SceneNode has been destroyed, this will cause undefined behavior.
         */
        GameObject& scale(const helios::math::vec3f& scale) noexcept {
            transform_->scale(scale);
            return *this;
        }

        /**
         * @brief Applies a rotation transformation to the GameObject.
         *
         * @param rotation The rotation matrix to apply.
         *
         * @return Reference to this GameObject for method chaining.
         *
         * @note This modifies the underlying SceneNode's local transform.
         *
         * @warning If the SceneNode has been destroyed, this will cause undefined behavior.
         */
        GameObject& rotate(const helios::math::mat4f& rotation) noexcept {
            transform_->rotate(rotation);
            return *this;
        }

        /**
         * @brief Applies a translation transformation to the GameObject.
         *
         * @param translation The translation vector to apply (x, y, z components).
         *
         * @return Reference to this GameObject for method chaining.
         *
         * @note This modifies the underlying SceneNode's local transform.
         *
         * @warning If the SceneNode has been destroyed, this will cause undefined behavior.
         */
        GameObject& translate(const helios::math::vec3f& translation) noexcept {
            transform_->translate(translation);
            return *this;
        }



    };


}