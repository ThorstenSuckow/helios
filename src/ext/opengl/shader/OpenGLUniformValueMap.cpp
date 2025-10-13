module;

#include <map>
#include <variant>

module helios.ext.opengl.rendering.shader.OpenGLUniformValueMap;

import helios.ext.opengl.rendering.shader.OpenGLUniformSemantics;

import helios.math.types;

namespace helios::ext::opengl::rendering::shader {

    void OpenGLUniformValueMap::set(OpenGLUniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) {
        map_.insert_or_assign(uniformSemantics, mat4f);
    }


    const float* OpenGLUniformValueMap::mat4f_ptr(OpenGLUniformSemantics uniformSemantics) const noexcept {

        if (auto it = map_.find(uniformSemantics); it != map_.end()) {
            if (const helios::math::mat4f* matPtr = std::get_if<helios::math::mat4f>(&it->second)) {
                return math::value_ptr(*matPtr);
            }
        }


        return nullptr;
    }

}