module;

export module helios.math.transform;

import helios.math.core;

export namespace helios::math {

    mat4 rotate(const mat4& model, float radians, const vec3& axis) noexcept;

};