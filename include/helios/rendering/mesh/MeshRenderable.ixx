/**
 * @file MeshRenderable.ixx
 * @brief Representative of a configurable MeshRenderable rendered by the underlying GL API.
 */
module;

#include <memory>
#include <optional>
#include <stdexcept>

export module helios.rendering.mesh.MeshRenderable;

import helios.rendering.mesh.MeshRenderCommand;

import helios.rendering.Renderable;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPrototype;
import helios.rendering.material.MaterialShaderPropertiesOverride;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::MeshRenderable"
export namespace helios::rendering::mesh {


    /**
     * @brief Represents a renderable object that combines a shared prototype with instance-specific overrides.
     *
     * A `MeshRenderable` aggregates an immutable `RenderPrototype` (shared asset definition)
     * with optional instance-specific `MaterialShaderPropertiesOverride`. This separation enables
     * efficient batching of shared prototypes while allowing per-instance visual adjustments.
     *
     * ## Design
     *
     * - **Shared Prototype:** Multiple `MeshRenderable` instances can reference the same `RenderPrototype`.
     * - **Per-Instance Overrides:** Each instance can customize material properties.
     * - **Move-Only:** Prevents accidental duplication during render queue processing.
     *
     * ## Data Flow
     *
     * ```
     * RenderPrototype (shared)
     *        │
     *        ├── MeshRenderable A (override: red color)
     *        ├── MeshRenderable B (override: blue color)
     *        └── MeshRenderable C (no override)
     * ```
     *
     * ## Performance Considerations
     *
     * **PERF-NOTE: shared_ptr overhead**
     *
     * The `renderPrototype_` member uses `std::shared_ptr`, which incurs atomic
     * reference counting overhead on every copy. In the hot rendering path, consider:
     *
     * - Returning `const RenderPrototype&` instead of `std::shared_ptr` from getters
     *   when ownership transfer is not required.
     * - Using `std::shared_ptr<T>::get()` for temporary access instead of copying
     *   the shared_ptr.
     * - Evaluating whether `std::unique_ptr` with raw pointer access would suffice
     *   if the prototype lifetime is well-defined (e.g., asset manager ownership).
     *
     * @see renderPrototype()
     *
     * @note For text rendering, use `TextRenderable` instead.
     *
     * @see RenderPrototype
     * @see RenderCommand
     * @see MaterialShaderPropertiesOverride
     */
    class MeshRenderable final : public helios::rendering::Renderable {

    protected:

        /**
         * @brief Shared pointer to the immutable RenderPrototype definition.
         */
        std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype_ = nullptr;

        /**
         * @brief The MaterialShaderPropertiesOverride owned by this class. This will be std::nullopt if not
         * available.
         */
        std::optional<helios::rendering::material::MaterialShaderPropertiesOverride> materialOverride_;

        /**
         * @brief Shared logger instance for all MeshRenderable objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:
        /**
         * @brief Deleted default constructor.
         */
        MeshRenderable() = delete;

        /**
         * @brief Deleted copy constructor.
         */
        MeshRenderable(const MeshRenderable&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        MeshRenderable& operator=(const MeshRenderable&)= delete;

        /**
         * @brief Defaulted move constructor.
         */
        MeshRenderable(MeshRenderable&&) noexcept = default;

        /**
         * @brief Defaulted move assignment operator.
         */
        MeshRenderable& operator=(MeshRenderable&&) noexcept = default;

        /**
         * @brief Creates a new MeshRenderable instance.
         *
         * @param renderPrototype A shared pointer to the immutable RenderPrototype definition. Must not be nullptr.
         * @param materialOverride An optional set of instance-specific material property overrides.
         * If std::nullopt, the MeshRenderable uses only the default properties from the RenderPrototype's Material.
         *
         * @throws std::invalid_argument if `renderPrototype` is a nullptr.
         */
        explicit MeshRenderable(
            std::shared_ptr<const helios::rendering::RenderPrototype> renderPrototype,
            const std::optional<helios::rendering::material::MaterialShaderPropertiesOverride>& materialOverride = std::nullopt
        ) :
            renderPrototype_(std::move(renderPrototype)),
            materialOverride_(materialOverride)
        {

            if (!renderPrototype_) {
                const std::string msg = "MeshRenderable constructor received a null shared pointer.";
                logger_.error(msg);
                throw std::invalid_argument(msg);
            }

        }


        /**
         * @brief Returns a shared pointer to the RenderPrototype used by this MeshRenderable.
         *
         * **PERF-NOTE:** This method returns a copy of the `shared_ptr`, which triggers
         * atomic reference count increment/decrement. If only read access is needed,
         * consider adding a `const RenderPrototype&` overload or using `renderPrototype_.get()`
         * directly for hot paths (e.g., during frame rendering).
         *
         * @return A shared pointer to the RenderPrototype.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::RenderPrototype> shareRenderPrototype() const noexcept {
            return renderPrototype_;
        }

        [[nodiscard]] const helios::rendering::RenderPrototype* renderPrototype() const noexcept {
            return renderPrototype_.get();
        }

        /**
         * @brief Returns a const reference to the optional instance-specific MaterialShaderPropertiesOverride.
         *
         * This allows read-only access to the overrides. Use `.has_value()` to check for existence
         * and `.value()` or `->` to safely access the override data.
         *
         * @return A const reference to the std::optional<MaterialShaderPropertiesOverride>.
         */

        [[nodiscard]] const std::optional<helios::rendering::material::MaterialShaderPropertiesOverride>& materialOverride() const noexcept {
            return materialOverride_;
        }

        /**
         * @brief Returns a non-const reference to the optional instance-specific MaterialShaderPropertiesOverride.
         *
         * This allows modification of the overrides. If the optional currently has no value,
         * it can be assigned to or `.emplace()` can be used to create a new MaterialShaderPropertiesOverride.
         *
         * @return A non-const reference to the std::optional<MaterialShaderPropertiesOverride>.
         */
        [[nodiscard]] std::optional<helios::rendering::material::MaterialShaderPropertiesOverride>& materialOverride() noexcept {
            return materialOverride_;
        }

        /**
         * @brief Returns true if this MeshRenderable was configured with a MaterialShaderPropertiesOverride.
         *
         * @return True if this MeshRenderable was configured with a MaterialShaderPropertiesOverride instance, otherwise false.
         */
        [[nodiscard]] bool hasMaterialOverride() const noexcept {
            return materialOverride_.has_value();
        }

        /**
         * @brief Returns the local-space axis-aligned bounding box.
         *
         * Delegates to the underlying mesh's AABB from the render prototype.
         *
         * @return A const reference to the local-space AABB.
         */
        [[nodiscard]] const helios::math::aabbf& localAABB() const noexcept override {
            return renderPrototype_->mesh().aabb();
        }

        /**
         * @brief Writes uniform values into the given map.
         *
         * This method first applies the default uniform values from the base Material definition and
         * then overlays any specific overrides provided by this instance's MaterialShaderPropertiesOverride.
         *
         * @param uniformValueMap Target map receiving the uniform values.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept override {
            renderPrototype_->material().writeUniformValues(uniformValueMap);

            if (materialOverride_) {
                materialOverride_->writeUniformValues(uniformValueMap);
            }
        }

        /**
         * @brief Emits a mesh render command to the render queue.
         *
         * Creates a `MeshRenderCommand` with the render prototype and uniform values,
         * then adds it to the render queue for processing by the rendering device.
         *
         * @param renderQueue The render queue to emit the command to.
         * @param objectUniformValues Object-specific uniform values (e.g., model matrix).
         * @param materialUniformValues Material uniform values (will be merged with material properties).
         */
        void emit(
            helios::rendering::RenderQueue& renderQueue,
            helios::rendering::shader::UniformValueMap objectUniformValues,
            helios::rendering::shader::UniformValueMap materialUniformValues) const override {

            writeUniformValues(materialUniformValues);

            renderQueue.add(helios::rendering::mesh::MeshRenderCommand(
                renderPrototype_.get(),
                std::move(objectUniformValues),
                std::move(materialUniformValues)
            ));
        };


    };

}

