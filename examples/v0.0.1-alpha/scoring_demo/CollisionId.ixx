/**
 * @file CollisionId.ixx
 * @brief Collision layer bitmask for the scoring demo.
 */
module;

#include <cstdint>

export module helios.examples.scoring.CollisionId;



export namespace helios::examples::scoring {

    enum CollisionId : uint32_t {

        None = 0,
        Player = 1 << 0,

        Projectile = 1 << 1,

        Enemy = 1 << 2,


        size_ = 4
    };


}