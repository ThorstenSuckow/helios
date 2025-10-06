module;

#include <list>
#include <memory>

export module helios.scene.SceneNode;

import helios.rendering.model.Node;
import helios.util.Guid;
import helios.math.types;

export namespace helios::scene {

    class SceneNode {

        private:
        std::shared_ptr<helios::rendering::model::Node> modelNode_;
        const util::Guid guid_;
        std::list<std::unique_ptr<SceneNode>> children_;
        math::mat4f localTranform_;

        public:
            explicit SceneNode(
                std::shared_ptr<helios::rendering::model::Node> modelNode
            ) noexcept :
                guid_(util::Guid::generate()),
                modelNode_(std::move(modelNode))
            {}

            [[nodiscard]] const helios::util::Guid& guid() const noexcept {
                return guid_;
            };

            /**
             * Scales this given the values in the component vector in model space.
             *
             * @param sc
             */
            void scale(helios::math::vec3f sc) noexcept {

            }

            /**
             * Rotates this node around the specified axis in model space.
             *
             * @param sc
             */
            void rotate(helios::math::vec3f sc) noexcept {

            }

            /**
             * Translates this node by the specified vector relative to its parent node.
             * If no parent is found, then translation is relative to the origin, i.e.
             * world coordinates.
             *
             * @param sc
             */
            void translate(helios::math::vec3f sc) noexcept {

            }


    };

}