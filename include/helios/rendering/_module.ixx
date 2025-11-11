/**
 * @file _module.ixx
 * @brief Module exports for rendering subsystem with primitives and device abstractions.
 */
export module helios.rendering;

/**
 * @brief Rendering system module for graphics rendering infrastructure.
 *
 * @details This module provides the core rendering system including rendering devices,
 * render commands, queues, passes, and vertex definitions. Also exports asset, model,
 * and shader management functionality for comprehensive 3D graphics rendering.
 */
export import helios.rendering.Vertex;
export import helios.rendering.RenderQueue;
export import helios.rendering.RenderPass;
export import helios.rendering.RenderPassFactory;
export import helios.rendering.RenderingDevice;
export import helios.rendering.RenderCommand;
export import helios.rendering.Renderable;
export import helios.rendering.shader;
export import helios.rendering.model;
export import helios.rendering.asset;
