/**
 * @file SceneNodeConfig.ixx
 * @brief Configuration for scene graph integration.
 */
module;

#include <cassert>
#include <memory>

export module helios.gameplay.builder.entity.builders.configs.SceneNodeConfig;

import helios.scene;
import helios.rendering;
import helios.ecs.components;
import helios.scene;

import helios.math;

export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for scene graph integration.
     *
     * @details Allows attaching a Entity's renderable to the scene graph
     * by creating a SceneNode and setting its parent and inheritance mode.
     * The config automatically manages HierarchyComponent relationships when
     * parenting to another Entity.
     *
     * ## Usage
     *
     * ```cpp
     * sceneConfig.parent(rootNode)
     *            .inherit(TransformType::Translation | TransformType::Rotation);
     * ```
     *
     * @see SceneNodeComponent
     * @see HierarchyComponent
     * @see TransformType
     */
    template<typename Entity>
    class SceneNodeConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;


        /**
         * @brief Validates that a RenderableComponent exists.
         */
        void ensureRenderableComponent() {
            const auto* renderableComponent = entity_.get<helios::rendering::components::RenderableComponent<Handle_type>>();
            assert(renderableComponent && "Unexpected nullptr for RenderableComponent.");
        }

        /**
         * @brief Validates SceneNodeComponent presence.
         *
         * @param shouldBeAvailable Whether the component should exist.
         */
        void ensureSceneNode(const bool shouldBeAvailable) {
            const auto* snc = entity_.get<helios::scene::components::SceneNodeComponent<Handle_type>>();

            if (shouldBeAvailable) {
                assert(snc && "Unexpected nullptr for SceneNodeComponent.");
            } else {
                assert(!snc && "Unexpected SceneNodeComponent.");
            }

        }

    public:

        /**
         * @brief Constructs a SceneNodeConfig for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit SceneNodeConfig(const Entity entity) : entity_(entity) {}

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

            const auto* renderableComponent = entity_.get<helios::rendering::components::RenderableComponent<Handle_type>>();

            auto renderable = renderableComponent->shareRenderable();

            auto node = std::make_unique<helios::scene::SceneNode>(renderable);
            auto node_ptr = parent->addNode(std::move(node));

            entity_.template add<helios::scene::components::SceneNodeComponent<Handle_type>>(node_ptr);

            return *this;
        }


        /**
         * @brief Creates a SceneNode and parents it to another Entity's node.
         *
         * @details Establishes both scene graph and ECS hierarchy relationships:
         * - Adds HierarchyComponent to both entities if not present
         * - Sets parent-child relationship in HierarchyComponent
         * - Parents the SceneNode to the parent Entity's SceneNode
         *
         * @param parent Parent Entity whose SceneNode will be the parent.
         *
         * @return Reference to this config for chaining.
         */
        SceneNodeConfig& parent(Entity parent) {
            ensureSceneNode(false);
            ensureRenderableComponent();

            parent.template getOrAdd<helios::ecs::components::HierarchyComponent<Handle_type>>().addChild(entity_.handle());
            entity_.template getOrAdd<helios::ecs::components::HierarchyComponent<Handle_type>>().setParent(parent.handle());

            const auto* renderableComponent = entity_.get<helios::rendering::components::RenderableComponent<Handle_type>>();

            auto renderable = renderableComponent->shareRenderable();

            auto node = std::make_unique<helios::scene::SceneNode>(renderable);

            auto* psn = parent.get<helios::scene::components::SceneNodeComponent<Handle_type>>();
            assert(psn && "Unexpected missing SceneNodeComponent for parent Entity");
            assert(psn->sceneNode() && "Unexpected missing SceneNode for parent Entity");

            auto node_ptr = psn->sceneNode()->addNode(std::move(node));

            entity_.template add<helios::scene::components::SceneNodeComponent<Handle_type>>(node_ptr);

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

            entity_.get<helios::scene::components::SceneNodeComponent<Handle_type>>()
                       ->sceneNode()
                       ->setInheritance(transformType) ;

            return *this;
        }



    };

}

