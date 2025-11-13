/**
 * @file Renderable.ixx
 * @brief Representative of a configurable Renderable rendered by the underlying GL API.
 */
module;

#include <memory>
#include <optional>
#include <stdexcept>

export module helios.rendering.Renderable;

import helios.rendering.RenderPrototype;
import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;


export namespace helios::rendering {


    /**
     * @brief Representative of a configurable Renderable rendered by the underlying GL API.
     *
     * A Renderable aggregates a reference to an immutable RenderPrototype (the shared asset definition)
     * with optional instance-specific material property overrides. This separation enables efficient
     * batching of shared RenderPrototypes while allowing individual visual adjustments per Renderable instance.
     *
     * Renderables are designed to be movable (e.g., in render queues) but not copyable to ensure unique
     * instance identity in processing.
     */
    class Renderable final {

    protected:

        std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype_ = nullptr;

        /**
         * @brief The MaterialPropertiesOverride owned by this class. This will be std::nullopt if not
         * available.
         */
        std::optional<helios::rendering::model::config::MaterialPropertiesOverride> materialOverride_;

        /**
         * @brief The logger used with this Renderable instance.
         */
        const helios::util::log::Logger* logger_ = nullptr;

    public:
        /**
         * @brief Remove default constructor.
         */
        Renderable() = delete;

        /**
         * @brief Delete copy constructor.
         */
        Renderable(const Renderable&) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Renderable& operator=(const Renderable&)= delete;

        // allow move
        Renderable(Renderable&&) noexcept = default;
        Renderable& operator=(Renderable&&) noexcept = default;

        /**
         * @brief Creates a new Renderable instance.
         *
         * @param renderPrototype A shared pointer to the immutable RenderPrototype definition. Must not be nullptr.
         * @param materialOverride An optional set of instance-specific material property overrides.
         * If nullptr, the Renderable uses only the default properties from the RenderPrototype's Material.
         * @param logger An (optional) pointer to a const logger instance. If not provided,
         * a default logger is automatically created.
         *
         * @throws std::invalid_argument if `renderPrototype` is a nullptr.
         */
        explicit Renderable(
            std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype,
            const std::optional<helios::rendering::model::config::MaterialPropertiesOverride>&
            materialOverride = std::nullopt,
            const helios::util::log::Logger* logger = nullptr
        );


        /**
         * @brief Returns a const reference to the immutable RenderPrototype used by this Renderable.
         *
         * @return A const reference to the RenderPrototype.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype() const noexcept;

        /**
         * @brief Returns a const reference to the optional instance-specific MaterialPropertiesOverride.
         *
         * This allows read-only access to the overrides. Use `.has_value()` to check for existence
         * and `.value()` or `->` to safely access the override data.
         *
         * @return A const reference to the std::optional<MaterialPropertiesOverride>.
         */
        [[nodiscard]] const std::optional<helios::rendering::model::config::MaterialPropertiesOverride>& materialOverride() const noexcept;

        /**
         * @brief Returns a non-const reference to the optional instance-specific MaterialPropertiesOverride.
         *
         * This allows modification of the overrides. If the optional currently has no value,
         * it can be assigned to or `.emplace()` can be used to create a new MaterialPropertiesOverride.
         *
         * @return A non-const reference to the std::optional<MaterialPropertiesOverride>.
         */
        [[nodiscard]] std::optional<helios::rendering::model::config::MaterialPropertiesOverride>& materialOverride() noexcept;

        /**
         * @brief Returns true if this Renderable was configured with a MaterialPropertiesOverride.
         *
         * @return True if this Renderable was configured with a MaterialPropertiesOverride instance, otherwise false.
         */
        [[nodiscard]] bool hasMaterialOverride() const noexcept;


        /**
         * @brief Writes uniform values into the given map.
         *
         * This method first applies the default uniform values from the base Material definition and
         * then overlays any specific overrides provided by this instance's MaterialPropertiesOverride.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept;



    };

}