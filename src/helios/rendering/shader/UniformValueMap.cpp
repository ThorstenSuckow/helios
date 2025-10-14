module;

#include <map>
#include <variant>

module helios.rendering.shader.UniformValueMap;

import helios.rendering.shader.UniformSemantics;

import helios.math.types;

namespace helios::rendering::shader {

    void UniformValueMap::set(UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) {
        map_.insert_or_assign(uniformSemantics, mat4f);
    }


    const float* UniformValueMap::mat4f_ptr(UniformSemantics uniformSemantics) const noexcept {

        if (auto it = map_.find(uniformSemantics); it != map_.end()) {
            if (const helios::math::mat4f* matPtr = std::get_if<helios::math::mat4f>(&it->second)) {
                return math::value_ptr(*matPtr);
            }
        }


        return nullptr;
    }

}