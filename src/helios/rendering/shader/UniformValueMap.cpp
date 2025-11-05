module;

#include <map>
#include <variant>

module helios.rendering.shader.UniformValueMap;

import helios.rendering.shader.UniformSemantics;

import helios.math.types;

namespace helios::rendering::shader {

    void UniformValueMap::set(const UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) noexcept {
        map_[std::to_underlying(uniformSemantics)].emplace(mat4f);
    }

    void UniformValueMap::set(const UniformSemantics uniformSemantics, const helios::math::vec4f& vec4f) noexcept {
        map_[std::to_underlying(uniformSemantics)].emplace(vec4f);
    }

    const float* UniformValueMap::mat4f_ptr(UniformSemantics uniformSemantics) const noexcept {

        const auto index = std::to_underlying(uniformSemantics);

        if (index >= map_.size()) {
            return nullptr;
        }

        if (const auto& el = map_[index]; el.has_value()) {
            if (const auto* it = std::get_if<helios::math::mat4f>(&el.value())) {
                return helios::math::value_ptr(*it);
            }
        }

        return nullptr;
    }

    const float* UniformValueMap::vec4f_ptr(UniformSemantics uniformSemantics) const noexcept {

        const auto index = std::to_underlying(uniformSemantics);

        if (index >= map_.size()) {
            return nullptr;
        }

        if (const auto& el = map_[index]; el.has_value()) {
            if (const auto* it = std::get_if<helios::math::vec4f>(&el.value())) {
                return helios::math::value_ptr(*it);
            }
        }

        return nullptr;
    }

}