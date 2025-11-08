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
        std::shared_ptr<MaterialInstance> materialInstance) :
        mesh_(std::move(mesh)),
        materialInstance_(std::move(materialInstance)) {
        if (!mesh_ || !materialInstance_) {
            const std::string msg = "Renderable constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] std::shared_ptr<const Mesh> Renderable::mesh() const noexcept {
        return mesh_;
    };


    [[nodiscard]] const MaterialInstance& Renderable::materialInstance() const noexcept {
        return *materialInstance_;
    };


}