module;

#include <memory>

import helios.rendering.core;
import helios.util.Guid;

export module helios.scene.Node;



export namespace helios::scene {

    class Node {

        private:
        std::shared_ptr<rendering::Mesh> mesh_;
        const util::Guid guid_;

        public:
            explicit Node() noexcept : guid_(util::Guid::generate()) {}

            const util::Guid& guid() const noexcept {
                return guid_;
            };

            Node& set_mesh(std::shared_ptr<rendering::Mesh> mesh) noexcept {
                mesh_ = std::move(mesh);
                return *this;
            }

            const std::shared_ptr<rendering::Mesh>& mesh() const noexcept {
                return mesh_;
            }

    };

}