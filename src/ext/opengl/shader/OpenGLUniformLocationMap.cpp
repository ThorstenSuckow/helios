module;

#include <cassert>
#include <format>
#include <map>


module helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;

import helios.rendering.shader.UniformSemantics;

namespace helios::ext::opengl::rendering::shader {

    bool OpenGLUniformLocationMap::set(
        const helios::rendering::shader::UniformSemantics uniformSemantics, int position
    ) noexcept {
        assert(position >= 0 && "position must not be less than 0");

        if (position < 0) {
            logger_.error(std::format("position must not be < 0, was {0}", position));
            return false;
        }

        const decltype(sentinel_) index = std::to_underlying(uniformSemantics);

        sentinel_ = (sentinel_ | (decltype(sentinel_){1} << index));
        map_[std::to_underlying(uniformSemantics)] = position;
        return true;
    }

    int OpenGLUniformLocationMap::get(const helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept {
        const decltype(sentinel_) index = std::to_underlying(uniformSemantics);

        if (!(sentinel_ & (decltype(sentinel_){1} << index))) {
            return -1;
        }

        return map_[index];
    }
};