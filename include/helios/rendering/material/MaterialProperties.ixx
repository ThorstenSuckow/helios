/**
 * @file MaterialProperties.ixx
 * @brief Legacy material property payload.
 */
module;

export module helios.rendering.material.MaterialProperties;


import helios.math.types;


export namespace helios::rendering::material {

    /**
     * @brief Simple material property container.
     */
    struct MaterialProperties {

        /**
         * @brief Base color (RGBA).
         */
        helios::math::vec4f baseColor = {1.0f, 1.0f, 1.0f, 1.0f};

        /**
         * @brief Surface roughness value.
         */
        float roughness = 0.0f;

    };
}
