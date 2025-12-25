/**
* @file SceneNodeComponent.ixx
 * @brief Component for linking GameObjects to the scene graph.
 */
module;

#include <cassert>

export module helios.engine.game.components.scene.SceneNodeComponent;

import helios.scene.SceneNode;
import helios.math.types;
import helios.core.units;

import helios.engine.game.Component;
import helios.engine.game.GameObject;

import helios.engine.game.components.model.ModelAabbComponent;


export namespace helios::engine::game::components::scene {

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
         * @brief The initial AABB of the model associated with the SceneNode.
         */
        helios::math::aabbf aabb_{};

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
            aabb_ = sceneNode->renderable() && sceneNode->renderable()->renderPrototype()
                  ? sceneNode->renderable()->renderPrototype()->mesh().aabb()
                  : helios::math::aabbf{};
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

            auto& msc = gameObject->getOrAdd<helios::engine::game::components::model::ModelAabbComponent>();

            msc.setAabb(aabb_);
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