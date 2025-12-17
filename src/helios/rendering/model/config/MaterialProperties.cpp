
module helios.rendering.model.config.MaterialProperties;

import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


namespace helios::rendering::model::config {


    MaterialProperties::MaterialProperties(
        const helios::math::vec4f baseColor,
        const float roughness
    ) noexcept : baseColor_(baseColor), roughness_(roughness){}


    MaterialProperties MaterialProperties::withBaseColor(const helios::math::vec4f baseColor) const noexcept {
        return MaterialProperties(
            baseColor,
            roughness_
        );
    }

    MaterialPropertiesOverride MaterialProperties::overrideBaseColor(const helios::math::vec4f baseColor) const noexcept {
        return MaterialPropertiesOverride(
            baseColor,
            roughness_
        );
    }

    MaterialProperties MaterialProperties::withRoughness(const float roughness) const noexcept {
        return MaterialProperties(
            baseColor_,
            roughness
        );
    }


    void MaterialProperties::writeUniformValues(shader::UniformValueMap& uniformValueMap) const noexcept {
        uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialBaseColor, baseColor_);
        uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialRoughness, roughness_);
    }
}
