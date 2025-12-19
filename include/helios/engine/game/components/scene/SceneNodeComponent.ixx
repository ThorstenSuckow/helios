/**
* @file SceneNodeComponent.ixx
 * @brief Component for linking GameObjects to the scene graph.
 */
module;

export module helios.engine.game.components.scene.SceneNodeComponent;

import helios.scene.SceneNode;
import helios.math.types;
import helios.core.units;
import helios.engine.game.Component;

export namespace helios::engine::game::components::scene {

    /**
     * @brief Component that links a GameObject to a SceneNode in the scene graph.
     *
     * @details Provides transform operations (translation, rotation, scale) on the
     * underlying SceneNode, as well as unit-aware sizing via setSize(). This component
     * bridges the game logic layer (GameObject) with the rendering layer (SceneNode).
     *
     * The canonical size is captured from the SceneNode's mesh AABB at construction
     * time. This enables setSize() to calculate the correct scale factors for
     * achieving a desired world-space size in Helios units.
     *
     * @note The SceneNode must remain valid for the lifetime of this component.
     */
    class SceneNodeComponent : public helios::engine::game::Component {

    protected:

        /**
         * @brief Original mesh dimensions from the SceneNode's AABB.
         *
         * @details Used as reference for setSize() calculations. Defaults to (1,1,1)
         *          if no renderable or mesh is available.
         */
        const helios::math::vec3f canonicalSize_{};

        /**
         * @brief Non-owning pointer to the associated SceneNode.
         */
        helios::scene::SceneNode* sceneNode_;


    public:

        /**
         * @brief Constructs a SceneNodeComponent linked to a SceneNode.
         *
         * @param sceneNode Pointer to the SceneNode to control. Must not be nullptr.
         */
        explicit SceneNodeComponent(helios::scene::SceneNode* sceneNode) :
            canonicalSize_(
                sceneNode->renderable() && sceneNode->renderable()->renderPrototype()
                ? sceneNode->renderable()->renderPrototype()->mesh().aabb().getSize()
                : helios::math::vec3f{1.0f, 1.0f, 1.0f}),
            sceneNode_(sceneNode)  {
        }
        

        /**
         * @brief Sets the world-space size of the object.
         *
         * @param width  Desired width in the specified unit.
         * @param height Desired height in the specified unit.
         * @param depth  Desired depth in the specified unit.
         * @param unit   Unit of measurement (default: Meter).
         *
         * @return Reference to this component for method chaining.
         *
         * @note Calculates scale factors based on canonical mesh size and applies them.
         */
        SceneNodeComponent& setSize(
            float width, float height, float depth, helios::core::units::Unit unit = helios::core::units::Unit::Meter
        ) noexcept {
            const auto& csize = canonicalSize_;

            setScale(
                helios::math::vec3f{
                    width != 0 && csize[0] != 0 ? helios::core::units::from(width, unit)/csize[0] : 0,
                    height != 0 && csize[1] != 0 ? helios::core::units::from(height, unit)/csize[1] : 0,
                    depth != 0 && csize[2] != 0 ? helios::core::units::from(depth, unit)/csize[2] : 0,
                }
            );

            return *this;
        };

        /**
         * @brief Sets the scale of the SceneNode directly.
         *
         * @param scale Scale factors for each axis.
         *
         * @return Reference to this component for method chaining.
         */
        SceneNodeComponent& setScale(const helios::math::vec3f& scale) noexcept {
            sceneNode_->setScale(scale);

            return *this;
        };

        /**
         * @brief Sets the rotation of the SceneNode.
         *
         * @param rotation Rotation matrix to apply.
         *
         * @return Reference to this component for method chaining.
         */
        SceneNodeComponent& setRotation(const helios::math::mat4f& rotation) noexcept {
            sceneNode_->setRotation(rotation);

            return *this;
        };

        /**
         * @brief Sets the translation of the SceneNode.
         *
         * @param translation Position vector in world space.
         *
         * @return Reference to this component for method chaining.
         */
        SceneNodeComponent& setTranslation(const helios::math::vec3f& translation) noexcept {
            sceneNode_->setTranslation(translation);

            return *this;
        };
        
        /**
         * @brief Returns the current local translation of the SceneNode.
         *
         * @return Translation vector in local space.
         */
        [[nodiscard]] helios::math::vec3f translation() const noexcept {
            return sceneNode_->localTransform().translation();
        }

        /**
         * @brief Returns the current local rotation of the SceneNode.
         *
         * @return Rotation as 4x4 matrix.
         */
        [[nodiscard]] helios::math::mat4f rotation() const noexcept {
            return sceneNode_->localTransform().rotation();
        }

        /**
         * @brief Returns the current local scale of the SceneNode.
         *
         * @return Scale factors for each axis.
         */
        [[nodiscard]] helios::math::vec3f scaling() const noexcept {
            return sceneNode_->localTransform().scaling();
        }

        /**
         * @brief Returns the underlying SceneNode.
         *
         * @return Non-owning pointer to the associated SceneNode.
         */
        [[nodiscard]] helios::scene::SceneNode* sceneNode() const noexcept {
            return sceneNode_;
        }
    };


}