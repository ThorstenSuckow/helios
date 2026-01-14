/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::modules namespace.
 *
 * @details This module aggregates all submodules of the modules namespace,
 * providing domain-specific components and systems for physics, spatial
 * transforms, scene integration, rendering, and object pooling.
 */
export module helios.engine.modules;

export import helios.engine.modules.physics;
export import helios.engine.modules.spatial;
export import helios.engine.modules.rendering;
export import helios.engine.modules.scene;
export import helios.engine.modules.pool;

