/**
 * @file NoCullingStrategy.ixx
 * @brief Culling strategy that intentionally keeps all scene members visible.
 */
module;

export module helios.scene.NoCullingStrategy;



import helios.ecs.types.EntityHandle;
import helios.scene.types.CullingContext;

using namespace helios::ecs::types;
using namespace helios::scene::types;
export namespace helios::scene {

    /**
     * @brief No-op culling strategy implementation.
     *
     * This strategy performs no visibility test and therefore does not remove
     * any entity from rendering consideration.
     *
     * @tparam TStrongId Strong-id type used by entity handles.
     */
    template<typename TMemberHandle>
    class NoCullingStrategy  {



    public:

        using MemberHandle_type = TMemberHandle;

        /**
         * @brief Executes no culling for the provided entity.
         *
         * @param ctx Perspective camera context.
         * @param handle Scene member handle that would be tested by culling strategies.
         *
         */
        bool shouldRender(const CullingContext<TMemberHandle>& ctx) noexcept {
            return true;
        }

    };
}