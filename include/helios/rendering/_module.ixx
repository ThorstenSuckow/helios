/**
 * @file _module.ixx
 * @brief Module exports for rendering subsystem with primitives and device abstractions.
 */
export module helios.rendering;

export import helios.rendering.Vertex;
export import helios.rendering.RenderQueue;
export import helios.rendering.RenderPass;
export import helios.rendering.ClearFlags;
export import helios.rendering.RenderPrototype;
export import helios.rendering.RenderPassFactory;
export import helios.rendering.RenderingDevice;
export import helios.rendering.Renderable;
export import helios.rendering.LegacyRenderPass;

export import helios.rendering.FramebufferToViewportMap;

export import helios.rendering.concepts;
export import helios.rendering.shader;
export import helios.rendering.material;
export import helios.rendering.asset;
export import helios.rendering.text;
export import helios.rendering.mesh;
export import helios.rendering.viewport;
export import helios.rendering.framebuffer;
export import helios.rendering.commands;
export import helios.rendering.model;
export import helios.rendering.components;