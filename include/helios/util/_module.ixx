/**
 * @file _module.ixx
 * @brief Module exports for non-domain-specific utility functions.
 */

/**
 * @brief Utility functions and helper classes.
 *
 * @details This module provides various utility functionality including GUID
 * generation, logging, and I/O operations used throughout the engine.
 */
export module helios.util;

export import helios.util.Guid;
export import helios.util.Colors;

export import helios.util.log;
export import helios.util.io;
