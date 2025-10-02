module;

#include <memory>
#include <stdexcept>

module helios.rendering.model;

import :Mesh;
import :Material;

import helios.util.log;

namespace helios::rendering::model {

    Node::Node(
        std::unique_ptr<Mesh> mesh,
        std::unique_ptr<Material> material) :
        mesh_(std::move(mesh)),
        material_(std::move(material)) {
        if (!mesh_ || !material_) {
            const std::string msg = "Node constructor received a null unique pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] const Mesh& Node::mesh() const noexcept {
        return *mesh_;
    };


    [[nodiscard]] const Material& Node::material() const noexcept {
        return *material_;
    };


}