module;

#include <memory>
#include <stdexcept>

module helios.rendering.Renderable;

import helios.rendering.model.Mesh;
import helios.rendering.model.Material;

import helios.util.log;

using namespace helios::rendering::model;

namespace helios::rendering {

    Renderable::Renderable(
        std::shared_ptr<Mesh> mesh,
        std::unique_ptr<Material> material) :
        mesh_(std::move(mesh)),
        material_(std::move(material)) {
        if (!mesh_ || !material_) {
            const std::string msg = "Renderable constructor received a null unique pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] std::shared_ptr<const Mesh> Renderable::mesh() const noexcept {
        return mesh_;
    };


    [[nodiscard]] const Material& Renderable::material() const noexcept {
        return *material_;
    };


}