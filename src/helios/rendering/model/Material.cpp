module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Material;

import helios.rendering.shader.Shader;
import helios.rendering.model.MaterialData;
import helios.rendering.shader.UniformValueMap;

using namespace helios::rendering::shader;

namespace helios::rendering::model {


    Material::Material(
        std::shared_ptr<const Shader> shader,
        std::shared_ptr<const MaterialData> materialData) :
        shader_(std::move(shader)),
        materialData_(std::move(materialData)) {
        if (!shader_) {
            const std::string msg = "MaterialData constructor received a null Shader shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
        if (!materialData_) {
            logger_.info("Material constructor received a null MaterialData shared pointer.");
        }
    }


    [[nodiscard]] std::shared_ptr<const Shader> Material::shader() const noexcept {
        return shader_;
    }


    [[nodiscard]] std::shared_ptr<const MaterialData> Material::materialData() const noexcept {
        return materialData_;
    }

    /**
     * @todo implement
     */
    void Material::writeUniformValues(UniformValueMap& uniformValueMap) const noexcept {
        /* intentionally left empty */
    };



}