module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.CombatConfig;


import helios.engine.modules.spatial.transform.components;

import helios.gameplay.damage.components;

using namespace helios::gameplay::damage::components;

export namespace helios::engine::builder::gameObject::builders::configs {


    template<typename Entity>
    class CombatConfig {

         Entity gameObject_;

    public:

        using Handle_type = typename Entity::Handle_type;

        explicit CombatConfig(Entity gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Tracks the last damage applied.
         *
         * @return Reference to this config for chaining.
         */
        CombatConfig& trackLastDamage() {
            gameObject_.template getOrAdd<LastDamageComponent<Handle_type>>();

            return *this;
        }

    };

}
