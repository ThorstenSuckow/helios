/**
 * @file _module.ixx
 * @brief Root module aggregating all helios subsystems.
 */

/**
 * @brief The helios game engine module.
 *
 * @details This is the root module that aggregates all subsystems of the helios framework,
 * including core functionality, mathematical operations, rendering, scene management,
 * input handling, event system, and application infrastructure. Importing this module
 * provides access to the entire helios engine API.
 */
export module helios;

export import helios.core;
export import helios.math;
export import helios.rendering;
export import helios.scene;
export import helios.input;
export import helios.engine;
export import helios.util;
export import helios.state;
export import helios.ui;
export import helios.physics;
export import helios.spatial;
export import helios.gameplay;
export import helios.runtime;
export import helios.platform;

export import helios.bootstrap;

