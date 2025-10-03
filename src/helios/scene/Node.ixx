module;

#include <memory>

export module helios.scene.Node;

import helios.rendering.model.Mesh;
import helios.util.Guid;

export namespace helios::scene {

    class Node {

        private:
        std::shared_ptr<helios::rendering::model::Mesh> mesh_;
        const util::Guid guid_;

        public:
            explicit Node() noexcept : guid_(util::Guid::generate()) {}

            const helios::util::Guid& guid() const noexcept {
                return guid_;
            };

            Node& set_mesh(std::shared_ptr<helios::rendering::model::Mesh> mesh) noexcept {
                mesh_ = std::move(mesh);
                return *this;
            }

            const std::shared_ptr<helios::rendering::model::Mesh>& mesh() const noexcept {
                return mesh_;
            }

    };

}