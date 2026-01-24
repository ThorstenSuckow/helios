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
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::Renderable"
export namespace helios::rendering {


    /**
     * @brief Representative of a configurable Renderable that references an immutable RenderPrototype
     * and instance specific material property overrides.
     *
     * A Renderable aggregates a reference to an immutable RenderPrototype (the shared asset definition)
     * with optional instance-specific material property overrides. This separation enables efficient
     * batching of shared RenderPrototypes while allowing individual visual adjustments per Renderable instance.
     *
     * The Renderable's interface is API-agnostic, enabling efficient batching and per-instance customization,
     * while abstracting the underlying API-specific rendering resources bundled within the RenderPrototype.
     *
     * Renderables are designed to be movable (e.g., in render queues) but not copyable to ensure unique
     * instance identity in processing.
     */
    class Renderable final {

    protected:

        /**
         * @brief Shared pointer to the immutable RenderPrototype definition.
         */
        std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype_ = nullptr;

        /**
         * @brief The MaterialPropertiesOverride owned by this class. This will be std::nullopt if not
         * available.
         */
        std::optional<helios::rendering::model::config::MaterialPropertiesOverride> materialOverride_;

        /**
         * @brief Shared logger instance for all Renderable objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:
        /**
         * @brief Deleted default constructor.
         */
        Renderable() = delete;

        /**
         * @brief Deleted copy constructor.
         */
        Renderable(const Renderable&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        Renderable& operator=(const Renderable&)= delete;

        /**
         * @brief Defaulted move constructor.
         */
        Renderable(Renderable&&) noexcept = default;

        /**
         * @brief Defaulted move assignment operator.
         */
        Renderable& operator=(Renderable&&) noexcept = default;

        /**
         * @brief Creates a new Renderable instance.
         *
         * @param renderPrototype A shared pointer to the immutable RenderPrototype definition. Must not be nullptr.
         * @param materialOverride An optional set of instance-specific material property overrides.
         * If std::nullopt, the Renderable uses only the default properties from the RenderPrototype's Material.
         *
         * @throws std::invalid_argument if `renderPrototype` is a nullptr.
         */
        explicit Renderable(
            std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype,
            const std::optional<helios::rendering::model::config::MaterialPropertiesOverride>&
            materialOverride = std::nullopt
        ) :
            renderPrototype_(std::move(renderPrototype)),
            materialOverride_(materialOverride)
        {

            if (!renderPrototype_) {
                const std::string msg = "Renderable constructor received a null shared pointer.";
                logger_.error(msg);
                throw std::invalid_argument(msg);
            }

        }


        /**
         * @brief Returns a shared pointer to the RenderPrototype used by this Renderable.
         *
         * @return A shared pointer to the RenderPrototype.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype() const noexcept {
            return renderPrototype_;
        }

        /**
         * @brief Returns a const reference to the optional instance-specific MaterialPropertiesOverride.
         *
         * This allows read-only access to the overrides. Use `.has_value()` to check for existence
         * and `.value()` or `->` to safely access the override data.
         *
         * @return A const reference to the std::optional<MaterialPropertiesOverride>.
         */

        [[nodiscard]] const std::optional<helios::rendering::model::config::MaterialPropertiesOverride>& materialOverride() const noexcept {
            return materialOverride_;
        }

        /**
         * @brief Returns a non-const reference to the optional instance-specific MaterialPropertiesOverride.
         *
         * This allows modification of the overrides. If the optional currently has no value,
         * it can be assigned to or `.emplace()` can be used to create a new MaterialPropertiesOverride.
         *
         * @return A non-const reference to the std::optional<MaterialPropertiesOverride>.
         */
        [[nodiscard]] std::optional<helios::rendering::model::config::MaterialPropertiesOverride>& materialOverride() noexcept {
            return materialOverride_;
        }

        /**
         * @brief Returns true if this Renderable was configured with a MaterialPropertiesOverride.
         *
         * @return True if this Renderable was configured with a MaterialPropertiesOverride instance, otherwise false.
         */
        [[nodiscard]] bool hasMaterialOverride() const noexcept {
            return materialOverride_.has_value();
        }


        /**
         * @brief Writes uniform values into the given map.
         *
         * This method first applies the default uniform values from the base Material definition and
         * then overlays any specific overrides provided by this instance's MaterialPropertiesOverride.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
            renderPrototype_->material().writeUniformValues(uniformValueMap);

            if (materialOverride_) {
                materialOverride_->writeUniformValues(uniformValueMap);
            }
        }



    };

}