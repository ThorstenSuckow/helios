module;

#include <cassert>
#include <iostream>
#include <map>


module helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;

import helios.rendering.shader.UniformSemantics;

namespace helios::ext::opengl::rendering::shader {

    bool OpenGLUniformLocationMap::set(const helios::rendering::shader::UniformSemantics uniformSemantics, int position) {
        assert(position >= 0 && "position must not be less than 0");

        if (position < 0) {
            logger_.error(std::format("position must not be < 0, was {0}", position));
            return false;
        }
        map_[uniformSemantics] = position;
        return true;
    }

    int OpenGLUniformLocationMap::get(const helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept {
        if (map_.contains(uniformSemantics)) {
            return map_.at(uniformSemantics);
        }

        return -1;
    }
};