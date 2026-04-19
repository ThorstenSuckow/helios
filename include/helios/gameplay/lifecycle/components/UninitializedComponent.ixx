/**
 * @file UninitializedComponent.ixx
 * @brief Lifecycle tag component that marks entities not initialized yet.
 */
module;

export module helios.gameplay.lifecycle.components.UninitializedComponent;

export namespace helios::gameplay::lifecycle::components {

    /**
     * @brief Tag component indicating that entity initialization is still pending.
     *
     * Initialization systems remove this marker once setup steps are completed.
     *
     * @tparam THandle Entity handle type associated with the owning world.
     */
    template<typename THandle>
    struct UninitializedComponent{};

}