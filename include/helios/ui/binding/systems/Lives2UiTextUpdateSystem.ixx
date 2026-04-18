/**
 * @file Lives2UiTextUpdateSystem.ixx
 * @brief System that synchronises a UI text element with a LivesComponent.
 */
module;


#include <format>



export module helios.ui.binding.systems.Lives2UiTextUpdateSystem;

import helios.gameplay.matchstate.components;
import helios.gameplay.matchstate.types;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;


import helios.ui.widgets;
import helios.ui.layout;

import helios.ecs.components.Active;

using namespace helios::gameplay::matchstate::components;
using namespace helios::gameplay::matchstate::types;

import helios.engine.common.tags.SystemRole;

export namespace helios::ui::binding::systems {

    /**
     * @brief Updates UiTextComponents bound to a LivesComponent via LivesBindingComponent.
     *
     * Each frame, the system compares the stored LivesRevision in the
     * binding against the target's current revision. On mismatch the
     * formatted lives value is written into the UiTextComponent.
     */
    template<typename THandle>
    class Lives2UiTextUpdateSystem {

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Synchronises bound text elements with their target lives values.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, lbc, dfc, txt, active] : updateContext.view<
                THandle,
                helios::gameplay::matchstate::components::LivesBindingComponent<THandle>,
                helios::ui::layout::components::NumberFormatterComponent<THandle>,
                helios::ui::widgets::components::UiTextComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                auto go = updateContext.find(lbc->target());

                if (!go) {
                    continue;
                }

                if (auto* lc = go->get<LivesComponent>()) {
                    if (lbc->livesRevision() != lc->livesRevision()) {
                        txt->setText(dfc->format(lc->lives()));
                        lbc->setLivesRevision(lc->livesRevision());
                    }
                }

            }
        }

    };

}
