/**
 * @file View.ixx
 * @brief Lightweight view for iterating entities with specific components.
 */
module;

#include <tuple>
#include <vector>
#include <functional>
#include <type_traits>

export module helios.engine.ecs.View;

import helios.engine.ecs.SparseSet;

import helios.engine.ecs.EntityManager;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.EntityHandle;
import helios.engine.core.data.EntityId;

export namespace helios::engine::ecs {

    /**
     * @brief A view class to iterate over entities having specific components.
     *
     * The View acts as a lightweight iterator over the SparseSets of the requested
     * components. It uses the first component type (Lead) as the primary iterator
     * and cross-references existence in other sets.
     *
     * ## Usage
     *
     * ```cpp
     * for (auto [entity, transform, velocity, active] : gameWorld->view<
     *     TransformComponent,
     *     VelocityComponent,
     *     Active
     * >().whereEnabled()) {
     *     // Process entity
     * }
     * ```
     *
     * @tparam Components The component types to query for.
     *
     * @see EntityManager
     * @see SparseSet
     */
    template<typename... Components>
    class View {

    private:
        EntityManager* em_;

        /**
         * @brief Pointers to the SparseSets of the included components.
         */
        std::tuple<SparseSet<Components>*... > includeSets_;

        /**
         * @brief List of exclusion predicates.
         * * Stores functions that return true if an entity should be EXCLUDED.
         * Operates on EntityId (index) because the SparseSet uses it internally.
         */
        std::vector<std::function<bool(EntityId)>> excludeChecks_;

        /**
         * @brief Flag to filter only enabled components.
         */
        bool filterEnabledOnly_ = false;

        bool filterActiveOnly_ = false;

    public:
        /**
         * @brief Constructs the view and retrieves the necessary component sets.
         *
         * @param em Pointer to the EntityManager to retrieve sets and construct GameObjects.
         */
        explicit View(EntityManager* em) : em_(em) {
            // Retrieve pointers to the specific component sets immediately.
            includeSets_ = std::make_tuple(em_->getSparseSet<Components>()...);
        };

        /**
         * @brief Excludes entities that have a specific component.
         *
         * @details Entities possessing the specified component type will be
         * skipped during iteration. Multiple exclusions can be chained.
         *
         * ```cpp
         * // Skip entities with Shield or Invincible
         * for (auto [e, health] : world->view<HealthComponent>()
         *     .exclude<ShieldComponent>()
         *     .exclude<InvincibleComponent>()) {
         *     // Only vulnerable entities
         * }
         * ```
         *
         * @tparam T The component type to exclude.
         *
         * @return Reference to this View for method chaining.
         */
        template<typename T>
        View& exclude() {
            auto* set = em_->getSparseSet<T>();

            if (set) {
                excludeChecks_.emplace_back([set](EntityId entityId) {
                    return set->contains(entityId);
                });
            }
            return *this;
        }

        /**
         * @brief Filters to only include entities with enabled components.
         *
         * @details Components must implement `isEnabled()` returning bool.
         * Components without this method are assumed to be enabled.
         *
         * @return Reference to this View for method chaining.
         */
        View& whereEnabled() {
            filterEnabledOnly_ = true;
            return *this;
        }

        /**
         * @brief Forward iterator for View traversal.
         *
         * @details Uses the first component type as the "lead" iterator and
         * validates each entity against all include/exclude/enabled criteria
         * before yielding.
         */
        struct Iterator {
            /**
             * @brief The first component type determines iteration order.
             */
            using LeadComponent = std::tuple_element_t<0, std::tuple<Components...>>;

            /**
             * @brief Iterator type from the lead component's SparseSet.
             */
            using LeadIterator  = typename SparseSet<LeadComponent>::Iterator;

            LeadIterator current_;
            LeadIterator end_;
            const View* view_;

            /**
             * @brief Default constructor creating an invalid iterator.
             */
            Iterator() = default;

            /**
             * @brief Constructs an iterator with the given range and view.
             *
             * @param current Iterator to the current position.
             * @param end Iterator to the end position.
             * @param view Pointer to the owning View for filter access.
             */
            Iterator(LeadIterator current, LeadIterator end, const View* view)
                : current_(current), end_(end), view_(view) {}

            /**
             * @brief Validates if the current entity matches all filter criteria.
             *
             * @details Performs the following checks in order:
             * 1. Entity validity in the registry
             * 2. Include check - entity has all required components
             * 3. Exclude check - entity has none of the excluded components
             * 4. Enabled check - all components pass isEnabled() (if filtered)
             *
             * @return True if the entity passes all checks, false otherwise.
             */
            [[nodiscard]] bool isValid() const {
                if (current_ == end_) {
                    return true;
                }

                // 1. Get Entity ID (from the Lead Iterator)
                EntityId entityId = current_.entityId();

                if (!view_->em_->isValid(entityId)) {
                    return false;
                }

                // 2. INCLUDE CHECK (Do we have all other required components?)
                // We iterate over the tuple of sets and check 'contains' for each.
                bool hasAllIncludes = std::apply([entityId](auto*... sets) {
                    return ((sets && sets->contains(entityId)) && ...);
                }, view_->includeSets_);

                if (!hasAllIncludes) {
                    return false;
                }

                // 3. EXCLUDE CHECK (Must NOT be present)
                for (const auto& excludeCheck : view_->excludeChecks_) {
                    if (excludeCheck(entityId)) {
                        return false; // If check returns true (has component), the entity is invalid.
                    }
                }

                // 4. ENABLED CHECK (State)
                if (view_->filterEnabledOnly_) {

                    // SFINAE Helper Lambda: Checks if .isEnabled() exists.
                    auto isComponentEnabled = [](const auto& comp) -> bool {
                        if constexpr (requires { comp.isEnabled(); }) {
                            return comp.isEnabled();
                        } else {
                            return true; // Assume enabled if method is missing.
                        }
                    };

                    // Check the Lead component (*current_ returns the component reference)
                    if (!isComponentEnabled(*current_)) {
                        return false;
                    }

                    // Check all other included components
                    bool allEnabled = std::apply([&](auto*... sets) {
                        // sets->get(id) returns a pointer, *ptr gives the reference.
                        return (isComponentEnabled(*sets->get(entityId)) && ...);
                    }, view_->includeSets_);

                    if (!allEnabled) {
                        return false;
                    }
                }

                return true;
            }

            /**
             * @brief Advances to the next valid entity.
             *
             * @details Increments the underlying iterator and skips invalid
             * entities until a valid one is found or end is reached.
             */
            void advance() {
                do {
                    ++current_;
                } while (current_ != end_ && !isValid());
            }

            /**
             * @brief Pre-increment operator.
             *
             * @return Reference to this iterator after advancing.
             */
            Iterator& operator++() noexcept {
                advance();
                return *this;
            }

            /**
             * @brief Inequality comparison.
             *
             * @param other The iterator to compare against.
             *
             * @return True if iterators point to different positions.
             */
            bool operator!=(const Iterator& other) const noexcept {
                return current_ != other.current_;
            }

            /**
             * @brief Dereference operator.
             *
             * @return A tuple containing {GameObject, Component*...}.
             *
             * @note Returns by value to support C++17 Structured Binding (auto [go, comp] : view).
             */
            [[nodiscard]] auto operator*() const {
                EntityId entityId = current_.entityId();

                auto handle = view_->em_->handle(entityId);


                return std::tuple_cat(
                    std::make_tuple(GameObject(handle, view_->em_)),
                    std::apply([entityId](auto*... sets) {
                        return std::make_tuple(sets->get(entityId)...);
                    }, view_->includeSets_)
                );
            }
        };

        /**
         * @brief Returns an iterator to the first valid entity.
         *
         * @details Uses the first component type's SparseSet as the lead.
         * If the first entity is invalid, advances to the next valid one.
         *
         * @return Iterator to the first valid entity, or end() if none found.
         */
        [[nodiscard]] Iterator begin() {
            auto* leadSet = std::get<0>(includeSets_);

            if (!leadSet) {
                return Iterator{};
            }

            Iterator it{leadSet->begin(), leadSet->end(), this};

            if (!it.isValid()) {
                it.advance();
            }

            return it;
        }

        /**
         * @brief Returns an iterator to the end (past the last entity).
         *
         * @return End iterator for comparison.
         */
        [[nodiscard]] Iterator end() {
            auto* leadSet = std::get<0>(includeSets_);

            if (!leadSet) {
                return Iterator{};
            }

            return Iterator{leadSet->end(), leadSet->end(), this};
        }
    };
}