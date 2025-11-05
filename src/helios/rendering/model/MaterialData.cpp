module;

#include <memory>
#include <string>
#include <stdexcept>

module helios.rendering.model.MaterialData;

import helios.rendering.shader.Shader;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialData"
namespace helios::rendering::model {

   const helios::math::vec4f& MaterialData::color() const noexcept {
       return color_;
   }

}