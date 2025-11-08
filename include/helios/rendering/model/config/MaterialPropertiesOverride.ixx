module;

#include <optional>

export module helios.rendering.model.config.MaterialPropertiesOverride;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::model::config {

    /**
     * @brief Configuration container for overriding specific material properties.
     *
     * This struct holds optional data values that can be used to override the default
     * properties defined in MaterialProperties used by a Material.
     * Only properties for which a value is present will be applied as overrides.
     */
    struct MaterialPropertiesOverride {


        /**
         * @brief Optional base color override for the material.
         */
        std::optional<helios::math::vec4f> baseColor;

        /**
         * @brief Writes this MaterialPropertiesOverride's values into the given map.
         *
         * This method is typically called **after** the Material's base properties were
         * written into the map.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {

            if (baseColor) {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialColor, *baseColor);
            }

        }
    };
}
