module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Material;

import helios.rendering.shader.Shader;
import helios.rendering.model.config.MaterialProperties;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

using namespace helios::rendering::shader;
using namespace helios::rendering::model::config;

namespace helios::rendering::model {


    Material::Material(
        std::shared_ptr<const Shader> shader,
        std::shared_ptr<const MaterialProperties> materialProperties) :
        shader_(std::move(shader)),
        materialProperties_(std::move(materialProperties)) {
        if (!shader_ || !materialProperties_) {
            const std::string msg = "Material constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    std::shared_ptr<const Shader> Material::shader() const noexcept {
        return shader_;
    }


    const MaterialProperties& Material::materialProperties() const noexcept {
        return *materialProperties_;
    }

    void Material::writeUniformValues(
    helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
        materialProperties_->writeUniformValues(uniformValueMap);
    }

}