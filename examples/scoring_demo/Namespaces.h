/**
 * @file Namespaces.h
 * @brief Common using-namespace declarations for the scoring demo.
 *
 * Include this header after all module imports to bring frequently
 * used namespaces into scope.
 */

#pragma once

// External backends
using namespace helios::ext::glfw::app;
using namespace helios::ext::glfw::window;
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
using namespace helios::rendering::asset::shape::basic;

// Input
using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::input::types;

// Core libraries
using namespace helios::math;
using namespace helios::scene;
using namespace helios::core::units;
using namespace helios::util;
using namespace helios::util::io;
using namespace helios::util::log;
using namespace helios::util::time;

// Engine core
using namespace helios::engine::bootstrap;
using namespace helios::engine::core::data;
using namespace helios::engine::state;
using namespace helios::engine::tooling;

// ECS
using namespace helios::engine::ecs::systems;

// Builder
using namespace helios::engine::builder::gameObject;

// Runtime
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::gameloop;
using namespace helios::engine::runtime::pooling;
using namespace helios::engine::runtime::spawn;

// Mechanics: state
using namespace helios::engine::mechanics::gamestate;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::gamestate::systems;
using namespace helios::engine::mechanics::match;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::mechanics::match::systems;

// Mechanics: scoring, timing, combat
using namespace helios::engine::mechanics::scoring;
using namespace helios::engine::mechanics::scoring::systems;
using namespace helios::engine::mechanics::scoring::types;
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::systems;
using namespace helios::engine::mechanics::combat::components;
using namespace helios::engine::mechanics::combat::systems;

// Mechanics: spawn, bounds, damage, health, input, lifecycle
using namespace helios::engine::mechanics::spawn::systems;
using namespace helios::engine::mechanics::spawn::types;
using namespace helios::engine::mechanics::bounds::systems;
using namespace helios::engine::mechanics::damage::systems;
using namespace helios::engine::mechanics::health::systems;
using namespace helios::engine::mechanics::health::types;
using namespace helios::engine::mechanics::input::systems;
using namespace helios::engine::mechanics::lifecycle::systems;
using namespace helios::engine::mechanics::health;

// Modules: physics
using namespace helios::engine::modules::physics::collision::types;
using namespace helios::engine::modules::physics::collision::systems;
using namespace helios::engine::modules::physics::motion::systems;
using namespace helios::engine::modules::physics::motion::components;

// Modules: spatial, scene, rendering, UI, AI
using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::modules::scene::systems;
using namespace helios::engine::modules::scene::components;
using namespace helios::engine::modules::scene::types;
using namespace helios::engine::modules::rendering::viewport::systems;
using namespace helios::engine::modules::ui;
using namespace helios::engine::modules::ui::widgets::systems;
using namespace helios::engine::modules::ui::widgets::commands;
using namespace helios::engine::modules::ui::widgets::types;
using namespace helios::engine::modules::ui::binding::systems;
using namespace helios::engine::modules::ui::transform::systems;
using namespace helios::engine::modules::ai::systems;

// Demo
using namespace helios::examples::scoring;

