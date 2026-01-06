/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game namespace.
 */
export module helios.engine.game;

// Core game loop types
export import helios.engine.game.Updatable;
export import helios.engine.game.UpdateContext;

// Component system
export import helios.engine.game.Component;
export import helios.engine.game.GameObject;
export import helios.engine.core.data.GameObjectFilter;
export import helios.engine.core.data.GameObjectPool;
export import helios.engine.game.GameWorld;

// level
export import helios.engine.game.Level;

// Command pattern
export import helios.engine.core.messaging.command.Command;
export import helios.engine.core.messaging.command.CommandBuffer;

// System base class
export import helios.engine.game.System;

// Aggregate sub-modules
export import helios.engine.game.gameplay;
export import helios.engine.game.physics;
export import helios.engine.game.spatial;
export import helios.engine.game.rendering;
export import helios.engine.game.scene;
export import helios.engine.game.model;
export import helios.engine.game.input;

