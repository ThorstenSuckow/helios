/**
 * @file Material.ixx
 * @brief Material abstraction: shader + material properties.
 */
module;

#include <memory>
#include <stdexcept>

export module helios.rendering.model.Material;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.model.config.MaterialProperties;
import helios.util.log;
import helios.rendering.shader.Shader;


export namespace helios::rendering::model {

    /**
     * @brief Represents a parameterizable material.
     *
     * A Material contains a reference to a shared, immutable `Shader` and
     * material properties used to configure the shader at draw time.
     */
    class Material final {


    protected:
        Material() = default;

        /**
         * @brief Shared pointer for the raw, immutable MaterialProperties.
         */
        std::shared_ptr<const helios::rendering::model::config::MaterialProperties> materialProperties_;

        /**
         * @brief Shared pointer to the immutable Shader.
         */
        std::shared_ptr<const helios::rendering::shader::Shader> shader_{};

        /**
         * @brief The logger used with this Material instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger* logger_;

    public:
        virtual ~Material() = default;

        /**
         * @brief Creates a new Material.
         *
         * @param shader A shared pointer to the immutable Shader used by this instance.
         * @param materialProperties A shared pointer to the MaterialProperties for this Material.
         * @param logger An optional Logger instance for logging. Will use a default logger if omitted.
         *
         * @throws std::invalid_argument if shader or materialProperties is a null shared pointer.
         */
        explicit Material(
            std::shared_ptr<const helios::rendering::shader::Shader> shader,
            std::shared_ptr<const helios::rendering::model::config::MaterialProperties> materialProperties,
            const helios::util::log::Logger* logger = nullptr
        );

        /**
         * @brief Returns a const reference to the underlying MaterialProperties.
         *
         * @return The const reference to this Material's MaterialProperties.
         */
        [[nodiscard]] const helios::rendering::model::config::MaterialProperties& materialProperties() const noexcept;


        /**
         * @brief Returns a const ref to the shader used by this instance.
         *
         * @return A const ref to the Shader used by this Material.
         */
        [[nodiscard]] const helios::rendering::shader::Shader& shader() const noexcept;


        /**
         * @brief Writes this Material's uniform values into the given map.
         *
         * Delegates to MaterialProperties::writeUniformValues().
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept;
    };

} // namespace helios::rendering::model