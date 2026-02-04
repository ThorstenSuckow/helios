/**
 * @file SceneNodeConfig.ixx
 * @brief Configuration for scene graph integration.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;

import helios.engine.ecs.GameObject;
import helios.scene;
import helios.engine.modules.rendering;
import helios.engine.modules.scene;

import helios.math;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for scene graph integration.
     *
     * Allows attaching a GameObject's renderable to the scene graph
     * by creating a SceneNode and setting its parent and inheritance mode.
     */
    class SceneNodeConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

        /**
         * @brief Validates that a RenderableComponent exists.
         */
        void ensureRenderableComponent() {
            const auto* renderableComponent = gameObject_->get<helios::engine::modules::rendering::renderable::components::RenderableComponent>();
            assert(renderableComponent && "Unexpected nullptr for RenderableComponent.");
        }

        /**
         * @brief Validates SceneNodeComponent presence.
         *
         * @param shouldBeAvailable Whether the component should exist.
         */
        void ensureSceneNode(const bool shouldBeAvailable) {
            const auto* snc = gameObject_->get<helios::engine::modules::scene::components::SceneNodeComponent>();

            if (shouldBeAvailable) {
                assert(snc && "Unexpected nullptr for SceneNodeComponent.");
            } else {
                assert(!snc && "Unexpected SceneNodeComponent.");
            }

        }

    public:

        /**
         * @brief Constructs a SceneNodeConfig for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SceneNodeConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Creates a SceneNode and parents it to the given node.
         *
         * @param parent Parent SceneNode in the scene graph.
         *
         * @return Reference to this config for chaining.
         */
        SceneNodeConfig& parent(helios::scene::SceneNode* parent) {
            ensureSceneNode(false);
            ensureRenderableComponent();

            const auto* renderableComponent = gameObject_->get<helios::engine::modules::rendering::renderable::components::RenderableComponent>();

            auto renderable = renderableComponent->shareRenderable();

            auto node = std::make_unique<helios::scene::SceneNode>(renderable);
            auto node_ptr = parent->addNode(std::move(node));

            gameObject_->add<helios::engine::modules::scene::components::SceneNodeComponent>(node_ptr);

            return *this;
        }

        /**
         * @brief Creates a SceneNode and parents it to another GameObject's node.
         *
         * @param parent Parent GameObject whose SceneNode will be the parent.
         *
         * @return Reference to this config for chaining.
         */
        SceneNodeConfig& parent(const helios::engine::ecs::GameObject* parent) {
            ensureSceneNode(false);
            ensureRenderableComponent();

            const auto* renderableComponent = gameObject_->get<helios::engine::modules::rendering::renderable::components::RenderableComponent>();

            auto renderable = renderableComponent->shareRenderable();

            auto node = std::make_unique<helios::scene::SceneNode>(renderable);

            auto* psn = parent->get<helios::engine::modules::scene::components::SceneNodeComponent>();
            assert(psn && "Unexpected missing SceneNodeComponent for parent GameObject");
            assert(psn->sceneNode() && "Unexpected missing SceneNode for parent GameObject");

            auto node_ptr = psn->sceneNode()->addNode(std::move(node));

            gameObject_->add<helios::engine::modules::scene::components::SceneNodeComponent>(node_ptr);

            return *this;
        }

        /**
         * @brief Sets the transform inheritance mode.
         *
         * @param transformType Which transforms to inherit from parent.
         *
         * @return Reference to this config for chaining.
         */
        SceneNodeConfig& inherit(helios::math::TransformType transformType) {
            ensureSceneNode(true);

            gameObject_->get<helios::engine::modules::scene::components::SceneNodeComponent>()
                       ->sceneNode()
                       ->setInheritance(transformType) ;

            return *this;
        }

    };

}

