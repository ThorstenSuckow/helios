/**
 * @file _module.ixx
 * @brief Module aggregator for helios.ecs.
 */

export module helios.ecs;


export import helios.ecs.concepts;
export import helios.ecs.types;
export import helios.ecs.strategies;
export import helios.ecs.components;

export import helios.ecs.ComponentOpsRegistry;
export import helios.ecs.ComponentReflector;
export import helios.ecs.Entity;
export import helios.ecs.EntityManager;
export import helios.ecs.EntityRegistry;
export import helios.ecs.EntityResolver;
export import helios.ecs.SparseSet;
export import helios.ecs.concepts.Traits;
export import helios.ecs.TypedHandleWorld;
export import helios.ecs.View;
