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

import helios.engine.ecs.CloneableComponent;
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
    class SceneNodeComponent : public helios::engine::ecs::CloneableComponent<SceneNodeComponent> {

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

        explicit SceneNodeComponent(const SceneNodeComponent& other) {

            auto* parent = other.sceneNode_->parent();

            assert(parent != nullptr && "unexpected nullptr for SceneNode's parent");

            auto sceneNode = std::make_unique<helios::scene::SceneNode>(other.sceneNode_->shareRenderable());

            sceneNode_ = parent->addNode(std::move(sceneNode));
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
        void onAttach(helios::engine::ecs::GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);

            assert(sceneNode_->shareRenderable() != nullptr && "Unexpected nullptr for SceneNode's renderable");

            // this will automatically create the RenderableComponent if not alreay registered
            gameObject->getOrAdd<helios::engine::modules::rendering::renderable::components::RenderableComponent>(sceneNode_->shareRenderable());
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