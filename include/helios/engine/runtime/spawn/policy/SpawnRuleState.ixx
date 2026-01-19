/**
 * @file SpawnRuleState.ixx
 * @brief Runtime state for a spawn rule.
 */
module;

export module helios.engine.runtime.spawn.policy.SpawnRuleState;

export namespace helios::engine::runtime::spawn::policy {

    /**
     * @brief Runtime state for a spawn rule.
     *
     * @details SpawnRuleState tracks the runtime state associated with a SpawnRule,
     * such as time since last spawn. This state is updated each frame and used by
     * SpawnCondition to determine if spawning should occur.
     *
     * @see SpawnRule
     * @see SpawnCondition
     */
    class SpawnRuleState {

        /**
         * @brief Time elapsed since the last spawn in seconds.
         */
        float sinceLastSpawn_ = 0.0f;

    public:

        /**
         * @brief Sets the time since last spawn.
         *
         * @param sinceLastSpawn Time in seconds.
         */
        void setSinceLastSpawn(float sinceLastSpawn) noexcept {
            sinceLastSpawn_ = sinceLastSpawn;
        }

        /**
         * @brief Returns the time since last spawn.
         *
         * @return Time in seconds since the last spawn.
         */
        [[nodiscard]] float sinceLastSpawn() const noexcept {
            return sinceLastSpawn_;
        }

        /**
         * @brief Updates the state with the frame delta time.
         *
         * @param deltaTime Time elapsed since last frame in seconds.
         */
        void update(const float deltaTime) noexcept {
            sinceLastSpawn_ += deltaTime;
        }

    };

}