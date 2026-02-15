/**
 * @file _module.ixx
 * @brief Match state management module for the helios engine.
 */
export module helios.engine.mechanics.match;

export import helios.engine.mechanics.match.components;
export import helios.engine.mechanics.match.systems;
export import helios.engine.mechanics.match.commands;
export import helios.engine.mechanics.match.dispatcher;
export import helios.engine.mechanics.match.types;
export import helios.engine.mechanics.match.listeners;
export import helios.engine.mechanics.match.rules;

export import helios.engine.mechanics.match.MatchStateCommandHandler;
export import helios.engine.mechanics.match.MatchStateManager;
export import helios.engine.mechanics.match.MatchStateTransitionListener;
