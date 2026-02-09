/**
* @file SceneNodeComponent.ixx
 * @brief Component for linking GameObjects to the scene graph.
 */
module;

#include <cassert>
#include <memory>


export module helios.engine.modules.scene.components.SceneNodeComponent;

import helios.scene.SceneNode;
import helios.math.types;
import helios.core.units.Unit;


import helios.engine.ecs.GameObject;

import helios.engine.modules.rendering.renderable.components.RenderableComponent;


export namespace helios::engine::modules::scene::components {

    /**
     * @brief Component that links a GameObject to a SceneNode in the scene graph.
     *
     * @details
     * This component bridges the game logic layer (GameObject) with the rendering layer (SceneNode).
     * It captures the initial AABB from the SceneNode's mesh (if available) and exposes
     * the underlying SceneNode for other components to use.
     *
     * @note The SceneNode must remain valid for the lifetime of this component.
     */
    class SceneNodeComponent  {

    protected:

        /**
         * @brief Non-owning pointer to the associated SceneNode.
         */
        helios::scene::SceneNode* sceneNode_;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Called when the owning GameObject is activated.
         *
         * @details Activates the SceneNode, making it visible in the scene graph.
         */
        void onActivate() noexcept {
            sceneNode_->setActive(true);
        }

        /**
         * @brief Called when the owning GameObject is deactivated.
         *
         * @details Deactivates the SceneNode, hiding it from the scene graph.
         */
        void onDeactivate() noexcept {
            sceneNode_->setActive(false);
        }



        /**
         * @brief Constructs a SceneNodeComponent linked to a SceneNode.
         *
         * @param sceneNode Pointer to the SceneNode to control. Must not be nullptr.
         */
        explicit SceneNodeComponent(helios::scene::SceneNode* sceneNode) :
            sceneNode_(sceneNode)  {

            assert(sceneNode_ != nullptr && "sceneNode must not be nullptr");
        }

        /**
         * @brief Copy constructor.
         *
         * @details Copies the SceneNode pointer. The actual SceneNode cloning
         * is handled by onClone() during entity cloning.
         */
        SceneNodeComponent(const SceneNodeComponent& other) = default;

        /**
         * @brief Copy assignment operator.
         */
        SceneNodeComponent& operator=(const SceneNodeComponent&)= default;

        /**
         * @brief Move constructor.
         */
        SceneNodeComponent(SceneNodeComponent&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        SceneNodeComponent& operator=(SceneNodeComponent&&) noexcept = default;

        /**
         * @brief Called after copy construction during entity cloning.
         *
         * @details Creates a new SceneNode for this entity by cloning the source's
         * renderable and adding it to the same parent node. This ensures each
         * cloned entity has its own SceneNode in the scene graph.
         *
         * @param other The source component to clone from.
         */
        void onClone(const SceneNodeComponent& other) {


            auto* parent = other.sceneNode_->parent();

            assert(parent != nullptr && "unexpected nullptr for SceneNode's parent");

            auto sceneNode = std::make_unique<helios::scene::SceneNode>(other.sceneNode_->shareRenderable());

            sceneNode_ = parent->addNode(std::move(sceneNode));
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