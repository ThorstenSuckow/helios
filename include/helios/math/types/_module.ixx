/**
 * @file _module.ixx
 * @brief Module exports for vector and matrix type definitions.
 */

/**
 * @brief Vector and matrix type definitions.
 *
 * @details This module provides fundamental mathematical types including 2D, 3D,
 * and 4D vectors, as well as 4x4 matrices used for graphics transformations
 * and spatial computations.
 */
export module helios.math.types;

export import :mat4;
export import :vec2;
export import :vec3;
export import :vec4;
export import :aabb;