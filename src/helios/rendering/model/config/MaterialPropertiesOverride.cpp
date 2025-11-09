module helios.rendering.model.config.MaterialPropertiesOverride;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

namespace helios::rendering::model::config {

    void MaterialPropertiesOverride::writeUniformValues(
        helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {

        if (baseColor) {
            uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialBaseColor, *baseColor);
        }

        if (roughness) {
            uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialRoughness, *roughness);
        }
    }

}
