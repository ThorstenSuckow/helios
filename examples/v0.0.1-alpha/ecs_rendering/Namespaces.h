/**
 * @file Namespaces.h
 * @brief Common using-namespace declarations for the scoring demo.
 *
 * Include this header after all module imports to bring frequently
 * used namespaces into scope.
 */

#pragma once

// External backends
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::ext::imgui;
using namespace helios::ext::imgui::widgets;

// Rendering
using namespace helios::rendering;
using namespace helios::rendering::mesh;
using namespace helios::rendering::material;
using namespace helios::rendering::shader;
using namespace helios::rendering::mesh::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::shader::types;
using namespace helios::rendering::shader::components;
using namespace helios::rendering::shader::systems;
using namespace helios::rendering::viewport;
using namespace helios::rendering::viewport::types;
using namespace helios::rendering::framebuffer;
using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::asset::shape::basic;

// Input
using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::input::types;

// Core libraries
using namespace helios::math;
using namespace helios::scene;
using namespace helios::scene::types;
using namespace helios::core::units;
using namespace helios::util::io;
using namespace helios::util::time;

// ECS library
using namespace helios::ecs;
using namespace helios::ecs::components;
using namespace helios::ecs::concepts;
using namespace helios::ecs::strategies;
using namespace helios::ecs::types;


// Engine core
using namespace helios::engine::bootstrap;
using namespace helios::engine::common::types;
using namespace helios::state;
using namespace helios::state::types;
using namespace helios::engine::tooling;


// Builder
using namespace helios::engine::builder::gameObject;

// Runtime
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::world;
using namespace helios::runtime::world::types;
using namespace helios::runtime::gameloop;
using namespace helios::runtime::pooling;
using namespace helios::gameplay::spawn;

// Platform
using namespace helios::platform::lifecycle;
using namespace helios::platform::lifecycle::commands;
using namespace helios::platform::lifecycle::systems;
using namespace helios::platform::environment;
using namespace helios::platform::environment::components;
using namespace helios::platform::environment::systems;
using namespace helios::platform::environment::types;
using namespace helios::platform::window;
using namespace helios::platform::window::components;
using namespace helios::platform::window::systems;
using namespace helios::platform::window::types;
using namespace helios::platform::glfw;
using namespace helios::platform::glfw::components;
using namespace helios::platform::glfw::systems;
using namespace helios::platform::opengl;

// Mechanics: scoring, timing, combat
using namespace helios::gameplay::scoring;
using namespace helios::gameplay::scoring::systems;
using namespace helios::gameplay::scoring::types;
using namespace helios::gameplay::timing;
using namespace helios::gameplay::timing::systems;
using namespace helios::gameplay::combat::components;
using namespace helios::gameplay::combat::systems;

// Mechanics: spawn, bounds, damage, health, input, lifecycle
using namespace helios::gameplay::spawn::systems;
using namespace helios::gameplay::spawn::types;
using namespace helios::gameplay::bounds::systems;
using namespace helios::gameplay::damage::systems;
using namespace helios::gameplay::health::systems;
using namespace helios::gameplay::health::types;
using namespace helios::gameplay::input::systems;
using namespace helios::gameplay::lifecycle::systems;
using namespace helios::gameplay::lifecycle::components;
using namespace helios::gameplay::health;
using namespace helios::gameplay::gamestate::types;
using namespace helios::gameplay::gamestate::systems;
using namespace helios::gameplay::matchstate::types;


// Modules: physics
using namespace helios::engine::modules::physics::collision::types;
using namespace helios::engine::modules::physics::collision::systems;
using namespace helios::engine::modules::physics::motion::systems;
using namespace helios::engine::modules::physics::motion::components;

// Modules: spatial, scene, rendering, UI, AI
using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::modules::spatial::transform::components;
using namespace helios::scene::systems;
using namespace helios::scene::components;
using namespace helios::scene;
using namespace helios::rendering::viewport::systems;
using namespace helios::rendering;
using namespace helios::rendering::components;
using namespace helios::rendering::commands;
using namespace helios::engine::modules::ui;
using namespace helios::engine::modules::ui::widgets::systems;
using namespace helios::engine::modules::ui::widgets::commands;
using namespace helios::engine::modules::ui::widgets::types;
using namespace helios::engine::modules::ui::binding::systems;
using namespace helios::engine::modules::ui::transform::systems;
using namespace helios::gameplay::ai::systems;

