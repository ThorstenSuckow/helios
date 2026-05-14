/**
 * @file CullingContext.ixx
 * @brief Context payload used for scene culling.
 */

module;



export module helios.scene.types.CullingContext;


import helios.math;

using namespace helios::math;
export namespace helios::scene::types {

    /**
     * @brief Groups the matrices, bounds, and handle needed for a culling test.
     *
     * @tparam TMemberHandle Scene member handle type.
     */
    template<typename TMemberHandle>
    struct CullingContext {

        /** @brief Projection matrix used for the culling pass. */
        mat4f projectionMatrix{1.0f};

        /** @brief View matrix used for the culling pass. */
        mat4f viewMatrix{1.0f};

        /** @brief World-space bounds of the tested scene member. */
        aabbf worldBounds{};

        /** @brief Handle of the tested scene member. */
        TMemberHandle handle;

    };

}