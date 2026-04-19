/**
 * @file InitializerList.ixx
 * @brief Composite initializer that chains multiple SpawnInitializers.
 */
module;

#include <array>
#include <memory>
#include <cstddef>

export module helios.gameplay.spawn.behavior.initializers.InitializerList;

import helios.gameplay.spawn.behavior.SpawnInitializer;
import helios.gameplay.spawn.types.SpawnPlanCursor;
import helios.gameplay.spawn.types.SpawnContext;


using namespace helios::gameplay::spawn::types;
export namespace helios::gameplay::spawn::behavior::initializers {

    /**
     * @brief Composite initializer that executes multiple SpawnInitializers in sequence.
     *
     * @details InitializerList allows combining multiple initialization behaviors into
     * a single initializer. Each initializer in the list is executed in order during
     * the spawn process.
     *
     * **Usage:**
     * ```cpp
     * auto initializer = std::make_unique<InitializerList<2>>(
     *     std::make_unique<DirectionInitializer>(vec3f{1, 0, 0}),
     *     std::make_unique<VelocityInitializer>(100.0f)
     * );
     * ```
     *
     * @tparam N The number of initializers in the list. Must match the number of
     *           arguments passed to the constructor.
     *
     * @see SpawnInitializer
     * @see SpawnContext
     */
    template<typename THandle, std::size_t N>
    class InitializerList {

        /**
         * @brief Fixed-size array of initializers to execute.
         */
        const std::array<std::unique_ptr<SpawnInitializer<THandle>>, N> initializers_;

    public:

        /**
         * @brief Constructs an InitializerList from variadic initializer arguments.
         *
         * @tparam Args Types of the initializer unique_ptrs.
         *
         * @param args Unique pointers to SpawnInitializer instances. The number of
         *             arguments must match the template parameter N.
         */
        template<typename... Args>
        requires (sizeof...(Args) == N && (std::derived_from<Args, SpawnInitializer<THandle>> && ...))
        explicit InitializerList(std::unique_ptr<Args>&& ...args) : initializers_{std::move(args)...}{}

        /**
         * @brief Executes all initializers in sequence on the spawned Entity.
         *
         * @param entity The Entity being initialized.
         * @param cursor The current spawn plan cursor with batch and index information.
         * @param spawnContext Context providing access to spawn-related data.
         */
        template<typename TEntity>
        requires std::is_same_v<typename TEntity::Handle_type, THandle>
        void initialize(
            TEntity entity,
            const SpawnPlanCursor& cursor,
            const SpawnContext<THandle>& spawnContext
        )  {
            for (std::size_t i = 0; i < initializers_.size(); ++i) {
                initializers_[i]->initialize(
                    entity,
                    cursor,
                    spawnContext
                );
            }
        }

    };

}


