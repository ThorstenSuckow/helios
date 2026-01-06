/**
* @file SceneNodeComponent.ixx
 * @brief Component for linking GameObjects to the scene graph.
 */
module;

#include <cassert>

export module helios.engine.game.scene.components.SceneNodeComponent;

import helios.scene.SceneNode;
import helios.math.types;
import helios.core.units.Unit;

import helios.engine.game.Component;
import helios.engine.game.GameObject;

import helios.engine.game.rendering.components.RenderableComponent;


export namespace helios::engine::game::scene::components {

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
    class SceneNodeComponent : public helios::engine::game::Component {

    protected:

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
            sceneNode_(sceneNode)  {

            assert(sceneNode_ != nullptr && "sceneNode must not be nullptr");
        }

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @details
         * Automatically adds or retrieves a ModelAabbComponent on the GameObject
         * and initializes it with the captured AABB from the SceneNode.
         *
         * @param gameObject Pointer to the parent GameObject.
         */
        void onAttach(helios::engine::game::GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);

            // this will automatically create the RenderableComponent if not alreay registered
            gameObject->getOrAdd<helios::engine::game::rendering::components::RenderableComponent>(sceneNode_->renderable());
        }

        /**
         * @brief Called when the owning GameObject becomes active.
         *
         * @details Activates the underlying SceneNode so it is included in rendering.
         */
        void onActivate() noexcept override {
            sceneNode_->setActive(true);
        }

        /**
         * @brief Called when the owning GameObject becomes inactive.
         *
         * @details Deactivates the underlying SceneNode so it is excluded from rendering.
         */
        void onDeactivate() noexcept override {
            sceneNode_->setActive(false);
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