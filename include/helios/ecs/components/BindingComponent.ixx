module;

#include <concepts>

export module helios.ecs.components.BindingComponent;

export namespace helios::ecs::components {

    /**
     * @brief Generic component that stores a handle reference to another entity.
     *
     * @details `BindingComponent` models a directed relationship from an owner entity
     * to a target entity via the target's handle type. It is used as a lightweight
     * link primitive for higher-level components such as scene, camera, and viewport
     * bindings.
     *
     * @tparam TOwnerHandle Handle type of the entity owning this component.
     * @tparam TTargetHandle Handle type of the referenced target entity.
     */
    template<typename TOwnerHandle, typename TTargetHandle>
    class BindingComponent {

        TTargetHandle targetHandle_{};

    public:

        /**
         * @brief Creates a binding from an explicit target handle.
         *
         * @param targetHandle Handle of the referenced target entity.
         */
        explicit BindingComponent(const TTargetHandle targetHandle) : targetHandle_(targetHandle) {};

        /**
         * @brief Creates a binding from a target entity instance.
         *
         * @details This constructor is enabled only if the entity's `Handle_type`
         * matches `TTargetHandle`.
         *
         * @tparam TTargetEntity Entity type exposing `Handle_type` and `handle()`.
         * @param targetEntity Referenced target entity.
         */
        template<typename TTargetEntity>
        requires std::same_as<TTargetHandle, typename TTargetEntity::Handle_type>
        explicit BindingComponent(const TTargetEntity targetEntity) : targetHandle_(targetEntity.handle()) {};

        /**
         * @brief Returns the bound target handle.
         *
         * @return Handle of the referenced target entity.
         */
        [[nodiscard]] TTargetHandle targetHandle() const noexcept {
            return targetHandle_;
        }
    };
}