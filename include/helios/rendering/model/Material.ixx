module;

#include <memory>
#include <stdexcept>

export module helios.rendering.model.Material;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.model.MaterialData;
import helios.util.log;

import helios.rendering.shader.Shader;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Material"
export namespace helios::rendering::model {

    /**
     * @brief Representative of a parametrizable Material instance.
     * A Material instance contains a reference to its raw shared Shader
     * and optionally MaterialData.
     * A Material instance allows for individual configurations.
     *
     * @see [RTR, pp. 125]
     */
    class Material {


    protected:
        Material() = default;

        /**
         * @brief Shared pointer for the raw, immutable MaterialData.
         */
        std::shared_ptr<const MaterialData> materialData_;

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
         * @brief Creates a new MaterialData instance.
         *
         * @param shader A shared pointer to the immutable Shader used by
         * this instance.
         * @param materialData An optional shared pointer to the MaterialData this
         * Material uses. Defaults to `nullptr`.
         *
         * @throws std::invalid_argument if shader is a null shared pointer.
         */
        explicit Material(
            std::shared_ptr<const helios::rendering::shader::Shader> shader,
            std::shared_ptr<const MaterialData> materialData = nullptr
        );

        /**
         * @brief Returns a shared pointer to the underlying MaterialData.
         * If this Material does not use MaterialData, this method returns a `nullptr`.
         *
         * @return The MaterialData of this Material, or `nullptr` if this Material has none.
         */
        [[nodiscard]] std::shared_ptr<const MaterialData> materialData() const noexcept;


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
         * @param uniformValueMap Target map receiving the uniform values.
         */
        virtual void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept;

    };

}