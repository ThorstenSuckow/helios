/**
 * @file ScoreValueConfig.ixx
 * @brief Configuration for score value components.
 */
module;

#include <cassert>
#include <type_traits>

export module helios.gameplay.builder.entity.builders.configs.ScoreValueConfig;


import helios.gameplay.scoring.components.ScoreValueComponent;

export namespace helios::gameplay::builder::entity::builders::configs {


    /**
     * @brief Fluent configuration for ScoreValueComponent setup.
     *
     * Provides a template method for adding ScoreValueComponent with
     * a specific score type (e.g., KillReward).
     */
    template<typename Entity>
    class ScoreValueConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a ScoreValueConfig.
         *
         * @param entity Target Entity to configure.
         */
        explicit ScoreValueConfig(Entity entity) : entity_(entity) {

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

            auto* svc = entity_.template get<helios::gameplay::scoring::components::ScoreValueComponent<Handle_type, T>>();

            assert(!svc && "Component already available.");

            entity_.template add<helios::gameplay::scoring::components::ScoreValueComponent<Handle_type, T>>(std::forward<Args>(args)...);

            return *this;
        }

    };

}
