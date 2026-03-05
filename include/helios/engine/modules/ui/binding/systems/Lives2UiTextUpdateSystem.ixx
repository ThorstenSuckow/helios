/**
 * @file Lives2UiTextUpdateSystem.ixx
 * @brief System that synchronises a UI text element with a LivesComponent.
 */
module;


#include <format>



export module helios.engine.modules.ui.binding.systems.Lives2UiTextUpdateSystem;

import helios.engine.mechanics.match.components;
import helios.engine.mechanics.match.types;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;


import helios.engine.modules.ui.widgets;
import helios.engine.modules.ui.layout;

import helios.engine.mechanics.lifecycle.components.Active;

using namespace helios::engine::mechanics::match::components;
using namespace helios::engine::mechanics::match::types;

import helios.engine.common.tags.SystemTag;

export namespace helios::engine::modules::ui::binding::systems {

    /**
     * @brief Updates UiTextComponents bound to a LivesComponent via LivesBindingComponent.
     *
     * Each frame, the system compares the stored LivesRevision in the
     * binding against the target's current revision. On mismatch the
     * formatted lives value is written into the UiTextComponent.
     */
    class Lives2UiTextUpdateSystem {

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemTag;
        /**
         * @brief Synchronises bound text elements with their target lives values.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, lbc, dfc, txt, active] : updateContext.view<
                helios::engine::mechanics::match::components::LivesBindingComponent,
                helios::engine::modules::ui::layout::components::NumberFormatterComponent,
                helios::engine::modules::ui::widgets::components::UiTextComponent,
                helios::engine::mechanics::lifecycle::components::Active
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
