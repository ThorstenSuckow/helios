/**
 * @file NoCullingStrategy.ixx
 * @brief Culling strategy that intentionally keeps all scene members visible.
 */
module;

export module helios.scene.NoCullingStrategy;



import helios.ecs.types.EntityHandle;
import helios.scene.types.PerspectiveCameraContext;

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
    template<typename TStrongId>
    class NoCullingStrategy  {



    public:

        /**
         * @brief Executes no culling for the provided entity.
         *
         * @param ctx Perspective camera context for compatibility with other strategies.
         * @param entityHandle Scene member handle that would be tested by culling strategies.
         */
        void cull(const PerspectiveCameraContext ctx, const EntityHandle<TStrongId>& entityHandle) const noexcept {
            // intentionally left empty
        }

    };
}