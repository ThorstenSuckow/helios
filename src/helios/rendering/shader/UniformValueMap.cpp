module;

#include <map>
#include <variant>

module helios.rendering.shader.UniformValueMap;

import helios.rendering.shader.UniformSemantics;

import helios.math.types;

namespace helios::rendering::shader {

    void UniformValueMap::set(UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) {
        map_[std::to_underlying(uniformSemantics)] = mat4f;
    }


    const float* UniformValueMap::mat4f_ptr(UniformSemantics uniformSemantics) const noexcept {

        const auto index = std::to_underlying(uniformSemantics);

        if (index >= map_.size()) {
            return nullptr;
        }

        if (const auto el = map_[index]; el.has_value()) {
            if (const auto* it = std::get_if<helios::math::mat4f>(&el.value())) {
                return helios::math::value_ptr(*it);
            }
        }

        return nullptr;
    }

}