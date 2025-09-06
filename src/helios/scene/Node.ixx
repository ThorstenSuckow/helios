module;

#include <memory>

import helios.rendering.core;
import helios.util.Guid;

export module helios.scene.Node;



export namespace helios::scene {

    class Node {

        private:
        std::shared_ptr<rendering::Mesh> _mesh;
        const util::Guid _guid;

        public:
            explicit Node() noexcept : _guid(util::Guid::generate()) {}

            const util::Guid& guid() const noexcept {
                return _guid;
            };

            Node& set_mesh(std::shared_ptr<rendering::Mesh> mesh) noexcept {
                _mesh = std::move(mesh);
                return *this;
            }

            std::shared_ptr<rendering::Mesh> mesh() const noexcept {
                return _mesh;
            }

    };

}