/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::mechanics namespace.
 *
 * @details This module aggregates all gameplay mechanics submodules,
 * providing high-level systems for bounds handling, combat, spawning,
 * and input processing.
 */
export module helios.engine.mechanics;

export import helios.engine.mechanics.bounds;
export import helios.engine.mechanics.combat;
export import helios.engine.mechanics.spawn;
export import helios.engine.mechanics.input;
