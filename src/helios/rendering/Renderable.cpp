module;

#include <memory>
#include <stdexcept>

module helios.rendering.Renderable;

import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.shader.UniformValueMap;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::rendering::Renderable"
namespace helios::rendering {

    using namespace helios::rendering::model;
    using namespace helios::util::log;
    using namespace helios::rendering::model::config;
    using namespace helios::rendering::shader;


    Renderable::Renderable(
        std::shared_ptr<const RenderPrototype> renderPrototype,
        const std::optional<MaterialPropertiesOverride>& materialOverride
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

    std::shared_ptr<const RenderPrototype> Renderable::renderPrototype() const noexcept {
        return renderPrototype_;
    }

    const std::optional<MaterialPropertiesOverride>& Renderable::materialOverride() const noexcept {
        return materialOverride_;
    }

    std::optional<MaterialPropertiesOverride>& Renderable::materialOverride() noexcept {
        return materialOverride_;
    }

    bool Renderable::hasMaterialOverride() const noexcept {
        return materialOverride_.has_value();
    }

    void Renderable::writeUniformValues(
        UniformValueMap& uniformValueMap) const noexcept {
        renderPrototype_->material().writeUniformValues(uniformValueMap);

        if (materialOverride_) {
            materialOverride_->writeUniformValues(uniformValueMap);
        }
    }


}