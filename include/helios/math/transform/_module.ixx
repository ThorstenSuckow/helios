/**
 * @file _module.ixx
 * @brief Module exports for transformation utilities.
 */

/**
 * @brief Transformation utilities for cameras, models, and scene graph inheritance.
 *
 * @details This module provides transformation functions and utilities including:
 * - **Camera transforms**: View matrix construction (`lookAt`)
 * - **Model transforms**: Rotation, scaling, translation utilities
 * - **TransformType**: Bitmask enum for selective transform inheritance
 *
 * @note Matrix operations `decompose()` and `transpose()` are available as
 * member functions of `mat4` in `helios.math.types`.
 */
export module helios.math.transform;

export import :camera;
export import :model;
