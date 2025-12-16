/**
 * @file _module.ixx
 * @brief Input handling module exports.
 */

/**
 * @brief Input handling module for user input management.
 *
 * @details This module provides input management functionality including adapters
 * for different input sources and managers for processing user input. Includes
 * type definitions for various input events and states.
 */
export module helios.input;

export import helios.input.types;
export import helios.input.gamepad;
export import helios.input.InputAdapter;
export import helios.input.InputManager;
