module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Material;

import helios.rendering.shader.Shader;
import helios.rendering.model.MaterialData;
import helios.rendering.shader.UniformValueMap;

namespace helios::rendering::model {


    Material::Material(std::shared_ptr<const helios::rendering::shader::Shader> shader) :
    shader_(std::move(shader)) {
        if (!shader_) {
            const std::string msg = "MaterialData constructor received a null Shader shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }

    }

    Material::Material(
        std::shared_ptr<const helios::rendering::shader::Shader> shader,
        std::shared_ptr<const MaterialData> materialData) :
        shader_(std::move(shader)),
        materialData_(std::move(materialData)) {
        if (!shader_) {
            const std::string msg = "MaterialData constructor received a null Shader shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
        if (!materialData_) {
            const std::string msg = "Material constructor received a null MaterialData shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] const helios::rendering::shader::Shader& Material::shader() const noexcept {
        return *shader_;
    }


    [[nodiscard]] const MaterialData& Material::materialData() const noexcept {
        return *materialData_;
    }


}