/**
 * @file MaterialProperties.ixx
 * @brief Default material properties used by materials and instances.
 */
module;

export module helios.rendering.model.config.MaterialProperties;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::model::config {

    /**
     * @brief Represents a Value Object for an immutable set of material properties.
     *
     * This class acts as a Value Object, encapsulating fundamental material characteristics
     * like base color and roughness. Once an instance is created, its properties cannot
     * be changed. Methods prefixed with `with*` facilitate functional updates by returning a
     * **new** MaterialProperties instance with the specified property modified, leaving the
     * original object - which may still be in use by existing Renderables - untouched.
     *
     */
    class MaterialProperties {

        private:
            /**
             * @brief Base color for the material.
             *
             * This value typically represents the diffuse color and includes an alpha
             * component for potential transparency.
             *
             * @see https://docs.omniverse.nvidia.com/materials-and-rendering/latest/templates/parameters/OmniPBR_Albedo.html
             */
            const helios::math::vec4f baseColor_ = {1.0f, 1.0f, 1.0f, 1.0f};

            /**
             * @brief The roughness factor of the material.
             *
             * This value determines the microfacet distribution of the surface, influencing how light is reflected.
             * A value of 0.0 represents a perfectly smooth, mirror-like surface. Conversely, 1.0 represents a
             * completely rough, matte surface.
             *
             * @see [573-575, PJH23]
             */
            const float roughness_ = 0.0f;



        public:
            /**
             * @brief Default destructor for MaterialProperties.
             */
            ~MaterialProperties() = default;

            /**
             * @brief Default constructor for MaterialProperties.
             *
             * initializes all properties to their default values.
             */
            MaterialProperties() = default;

            /**
             * @brief Constructs a MaterialProperties instance with the specified values.
             *
             * @param baseColor The initial base color for this MaterialProperties instance.
             * @param roughness The initial roughness factor for the material.
             */
            explicit MaterialProperties(
                const helios::math::vec4f baseColor,
                const float roughness
            ) noexcept;


            /**
             * Creates a new MaterialProperties instance with an updated base color.
             *
             * This method returns a new MaterialProperties instance where only the base color has been changed to the
             * provided value.
             *
             * @param baseColor The new base color for the material.
             *
             * @return A new immutable instance of MaterialProperties with the updated base color.
             */
            [[nodiscard]] MaterialProperties withBaseColor(helios::math::vec4f baseColor) const noexcept;

            /**
             * Creates a new MaterialProperties instance with an updated roughness factor.
             *
             * This method returns a new MaterialProperties instance where only the roughness factor has been changed to the
             * provided value.
             *
             * @param roughness The new roughness factor for the material.
             *
             * @return A new immutable instance of MaterialProperties with the updated roughness factor.
             */
            [[nodiscard]] MaterialProperties withRoughness(float roughness) const noexcept;


            /**
             * @brief Writes the current properties into the given UniformValueMap.
             *
             * @param uniformValueMap Target map receiving the uniform values.
             */
            void writeUniformValues(shader::UniformValueMap& uniformValueMap) const noexcept;

    };
}
