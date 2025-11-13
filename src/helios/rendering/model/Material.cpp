module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Material;

import helios.rendering.shader.Shader;
import helios.rendering.model.config.MaterialProperties;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;
import helios.util.log;

using namespace helios::rendering::shader;
using namespace helios::rendering::model::config;
using namespace helios::util::log;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Material"
namespace helios::rendering::model {


    Material::Material(
        std::shared_ptr<const Shader> shader,
        std::shared_ptr<const MaterialProperties> materialProperties,
        const Logger* logger
    ) :
        shader_(std::move(shader)),
        materialProperties_(std::move(materialProperties)),
        logger_(!logger ? &LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE) : logger)
    {
        if (!shader_ || !materialProperties_) {
            const std::string msg = !shader_ ?
                                    "Material constructor received a shader nullptr." :
                                    "Material constructor received a materialProperties nullptr.";
            logger_->error(msg);
            throw std::invalid_argument(msg);
        }
    }


    const Shader& Material::shader() const noexcept {
        return *shader_;
    }


    const MaterialProperties& Material::materialProperties() const noexcept {
        return *materialProperties_;
    }

    void Material::writeUniformValues(
    helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
        materialProperties_->writeUniformValues(uniformValueMap);
    }

}