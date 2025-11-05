module;

export module helios.rendering.model.MaterialData;

import helios.rendering.shader.Shader;
import helios.util.log.LogManager;
import helios.util.log.Logger;

import helios.math.types;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialData"
export namespace helios::rendering::model {

    /**
     * @brief Immutable MaterialData.
     *
     * MaterialData can safely be shared among Material instances.
     */
    class MaterialData {

    private:
        /**
         * @brief The color for this material data.
         */
        const helios::math::vec4f color_;

    public:
        virtual ~MaterialData() = default;

        explicit MaterialData(const helios::math::vec4f color) : color_(color){};

        /**
         * @brief Returns the base-color for this material.
         *
         * @return The helios::math::vec4f-vector containing the color information for this material.
         */
        [[nodiscard]] const helios::math::vec4f& color() const noexcept;

    };

}