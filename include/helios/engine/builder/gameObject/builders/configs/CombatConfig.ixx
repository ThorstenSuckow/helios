
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.CombatConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.spatial.transform.components;

import helios.engine.mechanics.damage.components;

using namespace helios::engine::mechanics::damage::components;

export namespace helios::engine::builder::gameObject::builders::configs {


    class CombatConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
         helios::engine::ecs::GameObject gameObject_;

    public:

        explicit CombatConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Tracks the last damage applied.
         *
         * @return Reference to this config for chaining.
         */
        CombatConfig& trackLastDamage() {
            gameObject_.getOrAdd<LastDamageComponent>();

            return *this;
        }

    };

}

