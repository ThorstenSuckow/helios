/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game namespace.
 */
export module helios.engine.game;

// Core game loop types
export import helios.engine.ecs.Updatable;
export import helios.engine.ecs.UpdateContext;

// Component system
export import helios.engine.ecs.Component;
export import helios.engine.ecs.GameObject;
export import helios.engine.ecs.query.GameObjectFilter;
export import helios.engine.core.data.GameObjectPool;
export import helios.engine.ecs.GameWorld;

// level
export import helios.engine.ecs.Level;

// Command pattern
export import helios.engine.core.messaging.command.TargetedCommand;
export import helios.engine.core.messaging.command.CommandBuffer;

// System base class
export import helios.engine.ecs.System;

// Aggregate sub-modules
export import helios.engine.game.gameplay;
export import helios.engine.game.physics;
export import helios.engine.game.spatial;
export import helios.engine.game.rendering;
export import helios.engine.game.scene;
export import helios.engine.game.model;
export import helios.engine.game.input;

