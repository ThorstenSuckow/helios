/**
 * @file FramebufferEntityManager.ixx
 * @brief Framebuffer-domain ECS registry and manager aliases.
 */
module;

#include "helios/helios_config.h"

export module helios.rendering.framebuffer.FramebufferEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.rendering.framebuffer.types;

constexpr auto FRAMEBUFFER_SIZE = DEFAULT_FRAMEBUFFER_POOL_CAPACITY;

using namespace helios::rendering::framebuffer::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::rendering::framebuffer {

    /**
     * @brief Entity registry used for framebuffer resources.
     */
    using FramebufferEntityRegistry = EntityRegistry<FramebufferDomainTag, LinearLookupStrategy<FRAMEBUFFER_SIZE>, true, FRAMEBUFFER_SIZE>;

    /**
     * @brief Entity manager used for framebuffer handles and framebuffer components.
     */
    using FramebufferEntityManager = EntityManager<FramebufferHandle, FramebufferEntityRegistry, FRAMEBUFFER_SIZE>;


}