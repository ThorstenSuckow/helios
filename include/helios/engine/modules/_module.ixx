/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::modules namespace.
 */
export module helios.engine.game;

// Core game loop types
export import helios.engine.ecs.Updatable;
export import helios.engine.runtime.world.UpdateContext;

// Component system
export import helios.engine.ecs.Component;
export import helios.engine.ecs.GameObject;
export import helios.engine.ecs.query.GameObjectFilter;
export import helios.engine.runtime.pooling.GameObjectPool;
export import helios.engine.runtime.world.GameWorld;

// level
export import helios.engine.runtime.world.Level;

// Command pattern
export import helios.engine.runtime.messaging.command.TargetedCommand;
export import helios.engine.runtime.messaging.command.CommandBuffer;

// System base class
export import helios.engine.ecs.System;

// Aggregate sub-modules
export import helios.engine.modules.gameplay;
export import helios.engine.modules.physics;
export import helios.engine.modules.spatial;
export import helios.engine.modules.rendering;
export import helios.engine.modules.scene;
export import helios.engine.modules.model;
export import helios.engine.modules.input;
export import helios.engine.modules.pool;

