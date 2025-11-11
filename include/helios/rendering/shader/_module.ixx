/**
 * @file _module.ixx
 * @brief Module exports for shader abstractions and utilities.
 */

/**
 * @brief Shader management and uniform handling.
 *
 * @details This module provides shader program management, uniform semantics,
 * and value mapping for shader parameter configuration in the rendering pipeline.
 */
export module helios.rendering.shader;

export import helios.rendering.shader.Shader;
export import helios.rendering.shader.UniformSemantics;
export import helios.rendering.shader.UniformValueMap;