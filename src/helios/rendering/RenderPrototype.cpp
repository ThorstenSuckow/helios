module;

#include <stdexcept>
#include <string>

module helios.rendering.RenderPrototype;

import helios.rendering.model.Mesh;
import helios.rendering.model.Material;

namespace helios::rendering {

    using namespace helios::rendering::model;

    RenderPrototype::RenderPrototype(
        std::shared_ptr<const Material> material,
        std::shared_ptr<const Mesh> mesh
    ) :
    material_(std::move(material)),
    mesh_(std::move(mesh)) {

        if (!material_) {
            throw std::invalid_argument("RenderPrototype received material nullptr");
        }
        if (!mesh_) {
            throw std::invalid_argument("RenderPrototype received mesh nullptr");
        }

    }

    const Mesh& RenderPrototype::mesh() const noexcept {
        return *mesh_;
    }


    const Material& RenderPrototype::material() const noexcept {
        return *material_;
    }

}