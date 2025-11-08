module;

export module helios.rendering.model.config.MaterialProperties;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::model::config {

    /**
     * @brief Configuration container for specific material properties.
     *
     * This struct holds the concrete data values (e.g. colors) that define the specific appearance of a
     * Material.
     *
     */
    struct MaterialProperties {


        /**
         * @brief Base color for the material.
         */
        helios::math::vec4f baseColor = {1.0f, 1.0f, 1.0f, 1.0f};

        /**
         * @brief Writes this MaterialProperties's values into the given map.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(shader::UniformValueMap& uniformValueMap) const noexcept {
            uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialColor, baseColor);
        }
    };
}
