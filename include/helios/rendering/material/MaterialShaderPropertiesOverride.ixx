/**
 * @file MaterialShaderPropertiesOverride.ixx
 * @brief Overrides for material properties to customize rendering parameters.
 */
module;

#include <optional>

export module helios.rendering.material.MaterialShaderPropertiesOverride;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::material {

    /**
     * @brief Configuration container for overriding specific material properties.
     *
     * This struct holds optional data values that can be used to override the default
     * properties defined in MaterialShaderProperties used by a Material.
     * Only properties for which a value is present will be applied as overrides.
     *
     * This struct is mutable by design. Changing its values will impact all MaterialInstances
     * using **this** instance.
     */
    struct MaterialShaderPropertiesOverride {


        /**
         * @brief Optional base color override for the material.
         */
        std::optional<helios::math::vec4f> baseColor;

        /**
         * @brief Optional roughness override for the material (range [0, 1]).
         */
        std::optional<float> roughness;

        /**
         * @brief Writes this MaterialShaderPropertiesOverride's values into the given map.
         *
         * This method is typically called **after** the Material's base properties were
         * written into the map.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {

            if (baseColor) {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialBaseColor, *baseColor);
            }

            if (roughness) {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialRoughness, *roughness);
            }
        }

    };
}
