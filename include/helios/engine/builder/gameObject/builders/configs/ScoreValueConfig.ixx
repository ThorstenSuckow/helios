/**
 * @file ScoreValueConfig.ixx
 * @brief Configuration for score value components.
 */
module;

#include <cassert>
#include <type_traits>

export module helios.engine.builder.gameObject.builders.configs.ScoreValueConfig;

import helios.engine.ecs.GameObject;

import helios.engine.mechanics.scoring.components.ScoreValueComponent;

export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for ScoreValueComponent setup.
     *
     * Provides a template method for adding ScoreValueComponent with
     * a specific score type (e.g., KillReward).
     */
    class ScoreValueConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a ScoreValueConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ScoreValueConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Adds a ScoreValueComponent with the specified score type.
         *
         * @tparam T The score type (must derive from Score).
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the score type constructor.
         *
         * @return Reference to this config for chaining.
         */
        template<typename T, typename... Args>
        ScoreValueConfig& score(Args&&... args) {

            auto* svc = gameObject_.get<helios::engine::mechanics::scoring::components::ScoreValueComponent<T>>();

            assert(!svc && "Component already available.");

            gameObject_.add<helios::engine::mechanics::scoring::components::ScoreValueComponent<T>>(std::forward<Args>(args)...);

            return *this;
        }

    };

}
