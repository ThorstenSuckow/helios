/**
 * @file _module.ixx
 * @brief Aggregate module for helios.engine.ecs.query.
 */
export module helios.engine.ecs.query;

export import helios.engine.ecs.query.GameObjectFilter;

// Note: GameObjectView is a .h header, not a module, due to MSVC ICE issues
// with structured bindings. Include via:
// #include <helios/engine/ecs/query/GameObjectView.h>

