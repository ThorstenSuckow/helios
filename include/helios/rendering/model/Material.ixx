module;

#include <memory>
#include <stdexcept>

export module helios.rendering.model.Material;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.model.config.MaterialProperties;
import helios.util.log;

import helios.rendering.shader.Shader;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Material"
export namespace helios::rendering::model {

    /**
     * @brief Representative of a parametrizable Material instance.
     * Material contains a reference to its raw shared Shader
     * and a shared MaterialConfig.
     */
    class Material {


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
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        virtual ~Material() = default;

        /**
         * @brief Creates a new Material.
         *
         * @param shader A shared pointer to the immutable Shader used by
         * this instance.
         * @param materialProperties A  shared pointer to the MaterialProperties for this Material.
         *
         * @throws std::invalid_argument if shader or materialConfig is a null shared pointer.
         */
        explicit Material(
            std::shared_ptr<const helios::rendering::shader::Shader> shader,
            std::shared_ptr<const helios::rendering::model::config::MaterialProperties> materialProperties
        );

        /**
         * @brief Returns a const ref  to the underlying MaterialProperties.
         *
         * @return The const ref to this Material's MaterialProperties.
         */
        [[nodiscard]] const helios::rendering::model::config::MaterialProperties& materialProperties() const noexcept;


        /**
         * @brief Returns a shared pointer to the shader used by this instance.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return The Shader this Material uses.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::shader::Shader> shader() const noexcept;


        /**
         * @brief Writes this Material's uniform values into the given map.
         *
         * Delegates to MaterialProperties::writeUniformValues()
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept;
    };

}