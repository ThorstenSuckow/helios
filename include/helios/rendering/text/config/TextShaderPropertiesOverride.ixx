
module;

#include <optional>

export module helios.rendering.text.config.TextShaderPropertiesOverride;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::text::config {


    struct TextShaderPropertiesOverride {


        std::optional<helios::math::vec4f> baseColor;


        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {

            if (baseColor) {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::TextColor, *baseColor);
            }

        }

    };
}
