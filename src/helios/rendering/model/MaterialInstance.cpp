module;

#include <memory>
#include <string>
#include <stdexcept>

module helios.rendering.model.MaterialInstance;

import helios.rendering.model.Material;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.Shader;

using namespace helios::rendering::shader;

namespace helios::rendering::model {

    MaterialInstance::MaterialInstance(
        std::shared_ptr<const helios::rendering::model::Material> material,
        std::shared_ptr<const helios::rendering::model::config::MaterialPropertiesOverride> materialOverrides
    ) :
        materialDefinition_(std::move(material)),
        materialOverrides_(std::move(materialOverrides)) {

        if (!materialDefinition_) {
            const std::string msg = "MaterialInstance constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    };

    std::shared_ptr<const Shader> MaterialInstance::shader() const noexcept {
        return materialDefinition_->shader();
    }


   void MaterialInstance::writeUniformValues(
        helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
        materialDefinition_->writeUniformValues(uniformValueMap);

        if (materialOverrides_) {
            materialOverrides_->writeUniformValues(uniformValueMap);
        }

   }

}