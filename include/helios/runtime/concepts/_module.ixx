/**
 * @file _module.ixx
 * @brief Aggregate module for `helios.runtime.concepts`.
 *
 * @details Re-exports all runtime concepts from their respective submodules.
 */
export module helios.runtime.concepts;

// Local concepts (runtime/concepts/)
export import helios.runtime.concepts.HasTag;
export import helios.runtime.concepts.HasReset;
export import helios.runtime.concepts.HasClear;
export import helios.runtime.concepts.HasSubmit;

// World concepts (runtime/world/concepts/)
export import helios.runtime.world.concepts;

// Messaging command concepts (runtime/messaging/command/concepts/)
export import helios.runtime.messaging.command.concepts;
