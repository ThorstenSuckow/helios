/**
 * @file algorithms.ixx
 * @brief Core algorithms and hash functions for the helios engine.
 */
module;

#include <cstdint>
#include <string>

export module helios.core.algorithms;



export namespace helios::core::algorithms {

    /**
     * @brief Computes a 32-bit FNV-1a hash from a string.
     *
     * @details Implements the Fowler–Noll–Vo hash function (FNV-1a variant),
     * a fast, non-cryptographic hash suitable for hash tables and identifier
     * generation. The function is `constexpr`, enabling compile-time hashing
     * of string literals.
     *
     * ## Algorithm
     *
     * 1. Initialize hash with FNV offset basis (2166136261)
     * 2. For each byte: XOR with hash, then multiply by FNV prime (16777619)
     *
     * ## Usage
     *
     * ```cpp
     * // Compile-time hash for type-safe IDs
     * constexpr uint32_t id = fnv1a_hash("enemy_spawn");
     *
     * // Used internally by strongly-typed ID constructors
     * constexpr GameObjectPoolId POOL{"bullets"};  // calls fnv1a_hash
     * ```
     *
     * @param str The string view to hash.
     *
     * @return 32-bit hash value.
     *
     * @note This is not a cryptographic hash. Do not use for security purposes.
     *
     * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
     * @see GameObjectPoolId
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    [[nodiscard]] constexpr uint32_t fnv1a_hash(const std::string_view str) noexcept {
        uint32_t hash = 2166136261U;
        for (char c: str) {
            hash ^= static_cast<uint8_t>(c);
            hash *= 16777619U;
        }
        return hash;
    }


}