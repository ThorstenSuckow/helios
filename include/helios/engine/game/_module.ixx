/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game namespace.
 */
export module helios.engine.game;

// Core game loop types
export import helios.engine.game.Updatable;
export import helios.engine.game.UpdateContext;
export import helios.engine.game.InputSnapshot;

// Component system
export import helios.engine.game.Component;
export import helios.engine.game.GameObject;
export import helios.engine.game.GameWorld;

// Command pattern
export import helios.engine.game.Command;
export import helios.engine.game.CommandBuffer;

// Aggregate sub-modules
export import helios.engine.game.commands;
export import helios.engine.game.components;
