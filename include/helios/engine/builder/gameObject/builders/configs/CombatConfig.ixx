
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.CombatConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.spatial.transform.components;

import helios.engine.mechanics.combat.components;

export namespace helios::engine::builder::gameObject::builders::configs {


    class CombatConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
         helios::engine::ecs::GameObject* gameObject_;

    public:

        explicit CombatConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Sets the weapon fire rate.
         *
         * @param fireRate Time in seconds between shots.
         *
         * @return Reference to this config for chaining.
         */
        CombatConfig& trackLastAttacker() {
            gameObject_->getOrAdd<helios::engine::mechanics::combat::components::LastAttackerComponent>();

            return *this;
        }

    };

}

