module;

#include <cassert>
#include <memory>

export module helios.gameplay.builder.entity.builders.configs.CombatConfig;


import helios.spatial.transform.components;

import helios.gameplay.damage.components;

using namespace helios::gameplay::damage::components;

export namespace helios::gameplay::builder::entity::builders::configs {


    template<typename Entity>
    class CombatConfig {

         Entity entity_;

    public:

        using Handle_type = typename Entity::Handle_type;

        explicit CombatConfig(Entity entity) : entity_(entity) {

        }

        /**
         * @brief Tracks the last damage applied.
         *
         * @return Reference to this config for chaining.
         */
        CombatConfig& trackLastDamage() {
            entity_.template getOrAdd<LastDamageComponent<Handle_type>>();

            return *this;
        }

    };

}
