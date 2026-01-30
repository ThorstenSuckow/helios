
module;

#include <memory>
#include <string>
#include <utility>

export module helios.rendering.text.config.TextShaderProperties;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

import helios.util.Colors;
import helios.math;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::text::config {

    class TextShaderProperties final {

    private:

            helios::math::vec4f baseColor_;

    public:


        explicit TextShaderProperties(
            const helios::math::vec4f baseColor = helios::util::Colors::White

        ) : baseColor_(baseColor) {}


        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
            uniformValueMap.set(helios::rendering::shader::UniformSemantics::TextColor, baseColor_);
        }
    };


}