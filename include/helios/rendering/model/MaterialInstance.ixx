module;

#include <memory>

export module helios.rendering.model.MaterialInstance;

import helios.rendering.model.Material;
import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.shader.Shader;
import helios.rendering.shader.UniformValueMap;
import helios.util.log.LogManager;
import helios.util.log.Logger;

import helios.math.types;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialInstance"
export namespace helios::rendering::model {

    /**
     * @brief MaterialInstance for providing Material definition and overrides
     * for the provided Material.
     *
     * This class represents a specific instance of a Material, combining a base Material
     * definition (including shader and default properties) with an optional MaterialPropertiesOverride
     * that specializes the Material's appearance for a particular object or use case.
     * This allows for efficient customization while reusing common material logic.
     *
     * @see [RTR, pp. 125]
     */
    class MaterialInstance {

    private:

        std::shared_ptr<const helios::rendering::model::Material> materialDefinition_;

        std::shared_ptr<const helios::rendering::model::config::MaterialPropertiesOverride> materialOverrides_;

        /**
         * @brief The logger used with this MaterialInstance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        virtual ~MaterialInstance() = default;

        /**
         * @brief Constructs a new Material instance.
         *
         * Initializes this instance with a base material definition and an optional set of property overrides.
         *
         * @param material A shared pointer to the base Material definition.
         * @param materialOverrides An optional shared pointer to a MaterialPropertiesOverride object,
         * containing values that override the defaults from the base material.
         * If nullptr, only the Material's defaults will be used.
         *
         * @throws std::invalid_argument if the shared ptr to material represents a nullptr.
         */
        explicit MaterialInstance(
            std::shared_ptr<const helios::rendering::model::Material> material,
            std::shared_ptr<const helios::rendering::model::config::MaterialPropertiesOverride> materialOverrides = nullptr
        );


        /**
         * @brief Returns a shared pointer to the shader used by the Material.
         *
         * Delegates to Material::shader().
         *
         * @return The Shader the underlying Material uses.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::shader::Shader> shader() const noexcept;


        /**
         * @brief Writes this MaterialInstance's uniform values into the given map.
         *
         * This method first applies the default uniform values from the base Material definition and
         * then overlays any any specific overrides provided by this instance's MaterialPropertiesOverride.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept;
    };

}