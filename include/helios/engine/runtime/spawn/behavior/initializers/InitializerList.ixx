/**
 * @file InitializerList.ixx
 * @brief Composite initializer that chains multiple SpawnInitializers.
 */
module;

#include <array>
#include <memory>

export module helios.engine.runtime.spawn.behavior.initializers.InitializerList;

import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;


export namespace helios::engine::runtime::spawn::behavior::initializers {

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
    template<std::size_t N>
    class InitializerList final : public SpawnInitializer {

        /**
         * @brief Fixed-size array of initializers to execute.
         */
        const std::array<std::unique_ptr<SpawnInitializer>, N> initializers_;

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
        requires (sizeof...(Args) == N && (std::derived_from<Args, SpawnInitializer> && ...))
        explicit InitializerList(std::unique_ptr<Args>&& ...args) : initializers_{std::move(args)...}{}

        /**
         * @brief Executes all initializers in sequence on the spawned GameObject.
         *
         * @param gameObject The GameObject being initialized.
         * @param cursor The current spawn plan cursor with batch and index information.
         * @param spawnContext Context providing access to spawn-related data.
         */
        void initialize(
            helios::engine::ecs::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {
            for (std::size_t i = 0; i < initializers_.size(); ++i) {
                initializers_[i]->initialize(
                    gameObject,
                    cursor,
                    spawnContext
                );
            }
        }

    };

}


