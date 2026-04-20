/**
 * @file _module.ixx
 * @brief Aggregate module for helios::gameplay namespace.
 *
 * @details This module aggregates all gameplay mechanics submodules,
 * providing high-level systems for bounds handling, combat, spawning,
 * and input processing.
 */
export module helios.gameplay;

export import helios.gameplay.ai;
export import helios.gameplay.builder;
export import helios.gameplay.effects;
export import helios.gameplay.bounds;
export import helios.gameplay.combat;
export import helios.gameplay.spawn;
export import helios.gameplay.input;
export import helios.gameplay.lifecycle;
export import helios.gameplay.damage;
export import helios.gameplay.health;
export import helios.gameplay.scoring;
export import helios.gameplay.gamestate;
export import helios.gameplay.matchstate;