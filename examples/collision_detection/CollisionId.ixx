module;

#include <cstdint>

export module helios.examples.collisionDetection.CollisionId;



export namespace helios::examples::collisionDetection {

    enum CollisionId : uint32_t {

        Player = 1 << 0,

        Projectile = 1 << 1,

        Enemy = 1 << 2

    };


}