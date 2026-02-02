/**
 * @file Material.ixx
 * @brief Material abstraction: shader + material properties.
 */
module;

#include <memory>
#include <stdexcept>

export module helios.rendering.material.Material;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.material.MaterialShaderProperties;
import helios.util.log;
import helios.rendering.shader.Shader;

#define HELIOS_LOG_SCOPE "helios::rendering::material::Material"
export namespace helios::rendering::material {

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
         * @brief Shared pointer for the raw, immutable MaterialShaderProperties.
         */
        std::shared_ptr<const helios::rendering::material::MaterialShaderProperties> materialProperties_;

        /**
         * @brief Shared pointer to the immutable Shader.
         */
        std::shared_ptr<const helios::rendering::shader::Shader> shader_{};

        /**
         * @brief Shared logger instance for all Material objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:
        virtual ~Material() = default;

        /**
         * @brief Creates a new Material.
         *
         * @param shader A shared pointer to the immutable Shader used by this instance.
         * @param materialProperties A shared pointer to the MaterialShaderProperties for this Material.
         *
         * @throws std::invalid_argument if shader or materialProperties is a null shared pointer.
         */
        explicit Material(
            std::shared_ptr<const helios::rendering::shader::Shader> shader,
            std::shared_ptr<const helios::rendering::material::MaterialShaderProperties> materialProperties
        ) :
            shader_(std::move(shader)),
            materialProperties_(std::move(materialProperties))
        {
            if (!shader_ || !materialProperties_) {
                const std::string msg = !shader_ ?
                                        "Material constructor received a shader nullptr." :
                                        "Material constructor received a materialProperties nullptr.";
                logger_.error(msg);
                throw std::invalid_argument(msg);
            }
        }

        /**
         * @brief Returns a const reference to the underlying MaterialShaderProperties.
         *
         * @return The const reference to this Material's MaterialShaderProperties.
         */
        [[nodiscard]] const helios::rendering::material::MaterialShaderProperties& materialProperties() const noexcept {
            return *materialProperties_;
        }


        /**
         * @brief Returns a const ref to the shader used by this instance.
         *
         * @return A const ref to the Shader used by this Material.
         */
        [[nodiscard]] const helios::rendering::shader::Shader& shader() const noexcept {
            return *shader_;
        }


        /**
         * @brief Writes this Material's uniform values into the given map.
         *
         * Delegates to MaterialShaderProperties::writeUniformValues().
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
            materialProperties_->writeUniformValues(uniformValueMap);
        }
    };

} // namespace helios::rendering::model