/**
 * @file MaterialShaderProperties.ixx
 * @brief Default material properties used by materials and instances.
 */
module;

export module helios.rendering.material.MaterialShaderProperties;

import helios.rendering.material.MaterialShaderPropertiesOverride;

import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::material {

    /**
     * @brief Represents a Value Object for an immutable set of material properties.
     *
     * This class acts as a Value Object, encapsulating fundamental material characteristics
     * like base color and roughness. Once an instance is created, its properties cannot
     * be changed. Methods prefixed with `with*` facilitate functional updates by returning a
     * **new** MaterialShaderProperties instance with the specified property modified, leaving the
     * original object - which may still be in use by existing Renderables - untouched.
     *
     */
    class MaterialShaderProperties {

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
             * @brief Default destructor for MaterialShaderProperties.
             */
            ~MaterialShaderProperties() = default;

            /**
             * @brief Default constructor for MaterialShaderProperties.
             *
             * initializes all properties to their default values.
             */
            MaterialShaderProperties() = default;

            /**
             * @brief Constructs a MaterialShaderProperties instance with the specified values.
             *
             * @param baseColor The initial base color for this MaterialShaderProperties instance.
             * @param roughness The initial roughness factor for the material, which defaults to 0.0f.
             */
            explicit MaterialShaderProperties(
                helios::math::vec4f baseColor,
                float roughness = 0.0f
            ) noexcept : baseColor_(baseColor), roughness_(roughness){}


            /**
             * Creates a new MaterialShaderProperties instance with an updated base color.
             *
             * This method returns a new MaterialShaderProperties instance where only the base color has been changed to the
             * provided value.
             *
             * @param baseColor The new base color for the material.
             *
             * @return A new immutable instance of MaterialShaderProperties with the updated base color.
             */
            [[nodiscard]] MaterialShaderProperties withBaseColor(helios::math::vec4f baseColor) const noexcept {
                return MaterialShaderProperties(
                    baseColor,
                    roughness_
                );
            }

            /**
             * Creates a new MaterialShaderPropertiesOverride instance with an updated base color.
             *
             * This method returns a new MaterialShaderPropertiesOverride instance where only the base
             * color has been changed to the provided value.
             *
             * @param baseColor The new base color for the material.
             *
             * @return A new instance of MaterialShaderPropertiesOvrride with the updated base color.
             */
            [[nodiscard]] MaterialShaderPropertiesOverride overrideBaseColor(helios::math::vec4f baseColor) const noexcept {
                return MaterialShaderPropertiesOverride(
                    baseColor,
                    roughness_
                );
            }

            /**
             * Creates a new MaterialShaderProperties instance with an updated roughness factor.
             *
             * This method returns a new MaterialShaderProperties instance where only the roughness factor has been changed to the
             * provided value.
             *
             * @param roughness The new roughness factor for the material.
             *
             * @return A new immutable instance of MaterialShaderProperties with the updated roughness factor.
             */
            [[nodiscard]] MaterialShaderProperties withRoughness(float roughness) const noexcept {
                return MaterialShaderProperties(
                    baseColor_,
                    roughness
                );
            }


            /**
             * @brief Writes the current properties into the given UniformValueMap.
             *
             * @param uniformValueMap Target map receiving the uniform values.
             */
            void writeUniformValues(shader::UniformValueMap& uniformValueMap) const noexcept {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialBaseColor, baseColor_);
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::MaterialRoughness, roughness_);
            }

    };
}
