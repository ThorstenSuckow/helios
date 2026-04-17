/**
 * @file GLFWPlatformManager.ixx
 * @brief GLFW-backed platform manager handling runtime init, window lifecycle, and frame service commands.
 */
module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <ranges>
#include <unordered_map>
#include <vector>

export module helios.platform.glfw.GLFWPlatformManager;


import helios.util.log;
import helios.ecs.types.EntityHandle;
import helios.core.types;
import helios.engine.state.Bindings;


import helios.engine.state.commands;
import helios.engine.state.types;
import helios.engine.mechanics.gamestate.types;

import helios.engine.common.tags.ManagerRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.platform.environment.commands;
import helios.platform.environment.components;
import helios.platform.environment.types;

import helios.platform.window.commands;
import helios.platform.window.components;
import helios.platform.window.types;

import helios.platform.glfw.components;
import helios.platform.glfw.types;

import helios.ecs.concepts;
import helios.engine.common.concepts;
import helios.engine.runtime.messaging.command;

using namespace helios::engine::common::tags;
using namespace helios::platform::environment::commands;
using namespace helios::platform::environment::types;
using namespace helios::platform::environment::components;
using namespace helios::platform::window::commands;
using namespace helios::platform::window::types;
using namespace helios::platform::window::components;
using namespace helios::platform::glfw::components;
using namespace helios::platform::glfw::types;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;
using namespace helios::ecs;
using namespace helios::ecs::concepts;
using namespace helios::engine::common::concepts;
using namespace helios::core::types;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::world;
    
#define HELIOS_LOG_SCOPE "helios::platform::glfw::GLFWPlatformManager"
export namespace helios::platform::glfw {

    /**
     * @brief Concrete manager integrating GLFW with helios runtime/window command flow.
     *
     * The manager receives runtime and window commands via `submit(...)`, stores them
     * as pending work, and processes them in `flush(...)` in a deterministic order.
     *
     * @tparam THandle Window/entity handle type.
     * @tparam TCommandBuffer Command buffer used for follow-up state/runtime commands.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsEntityHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class GLFWPlatformManager {

        GameWorld* gameWorld_ = nullptr;

        std::vector<WindowResizeCommand<THandle>> pendingResizeCommands_;

        std::vector<WindowCreateCommand<THandle>> windowCreateCommands_;

        std::vector<SwapBuffersCommand<THandle>> pendingBufferSwaps_;

        std::vector<WindowCloseCommand<THandle>> pendingCloseCommands_;

        std::vector<THandle> currentContexts_;

        bool shouldInit_ = false;

        bool shouldShutdown_ = false;

        bool pollEvents_ = false;

        bool initialized_ = false;

        bool openGLLoaded_ = false;

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
                   HELIOS_LOG_SCOPE);


        void initPlatform(UpdateContext& updateContext) noexcept {

            if (!shouldInit_ || initialized_) {
                return;
            }

            if (glfwInit() == GLFW_FALSE) {
                assert(false && "Failed to initialize glfw");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            assert(updateContext.session().state<GameState>() == GameState::Booting &&
                "Expected GameState to be Booting during platform initialization");

            initialized_ = updateContext.session().initialize() &&
                           updateContext.runtimeEnvironment().initialize();

            updateContext.queueCommand<TCommandBuffer, StateCommand<GameState>>(
                StateTransitionRequest<GameState>(
                    updateContext.session().state<GameState>(),
                    GameStateTransitionId::BootRequest
                )
            );

            shouldInit_ = false;
        }


        bool createWindow(UpdateContext& updateContext, const WindowCreateCommand<THandle>& cmd) noexcept {

            assert(gameWorld_ && "GameWorld not initialized");

            auto window = updateContext.find(cmd.windowHandle);

            if (!window) {
                return false;
            }

            auto& cfg = cmd.windowConfig;

            auto* nativeHandle = glfwCreateWindow(
                cfg.size.width,
                cfg.size.height,
                cfg.title.c_str(),
                nullptr,
                nullptr
            );

            assert(nativeHandle && "Failed to create GLFW window");

            if (cfg.aspectRatioNumer > 0 && cfg.aspectRatioDenom > 0) {
                glfwSetWindowAspectRatio(
                    nativeHandle,
                    cfg.aspectRatioNumer,
                    cfg.aspectRatioDenom
                );
            }

            assert(window->has<WindowCreateRequestComponent<THandle>>() && "Expected entity to have WindowCreateRequestComponent");
            window->remove<WindowCreateRequestComponent<THandle>>();
            assert(!window->has<WindowCreateRequestComponent<THandle>>() && "Expected entity to not have WindowCreateRequestComponent");
            assert(!window->has<WindowComponent<THandle>>() && "Expected entity to not have WindowComponent");
            window->add<WindowComponent<THandle>>(
                std::move(cfg.title),
                WindowSize{cfg.size.width, cfg.size.height},
                cfg.aspectRatioNumer,
                cfg.aspectRatioDenom
            );
            window->add<GLFWWindowHandleComponent<THandle>>(nativeHandle);

            removeCurrentContext(updateContext);

            glfwMakeContextCurrent(nativeHandle);

            window->add<CurrentContextComponent<THandle>>();

            window->add<WindowShownComponent<THandle>>();
            window->add<GLFWWindowUserPointerComponent<THandle>>(GLFWWindowUserPointer<THandle>(cmd.windowHandle, gameWorld_));

            installResizeListener(gameWorld_, cmd.windowHandle);

            return true;
        }

        bool initOpenGL(UpdateContext& updateContext) noexcept {

            const GLADloadfunc procAddressLoader = glfwGetProcAddress;
            const int gl_ver = gladLoadGL(procAddressLoader);

            if (gl_ver == 0) {
                logger_.error("Failed to load OpenGL");
                assert(false && "Failed to load OpenGL");
                return false;
            }

            logger_.info(std::format("OpenGL {0}.{1} loaded", GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver)));

            updateContext.runtimeEnvironment().setGPUReady();

            return true;
        }

        void removeCurrentContext(UpdateContext& updateContext) {
            // remove any currentcontext component
            currentContexts_.clear();
            for (auto [window, cc]: updateContext.template view<THandle, CurrentContextComponent<THandle>>()) {
                currentContexts_.push_back(window.handle());
            }
            for (auto& handle : currentContexts_) {
                auto go = updateContext.find<THandle> (handle);
                if (go) {
                    go->remove<CurrentContextComponent<THandle>>();
                }
            }
        }

        void installResizeListener(GameWorld* gameWorld, THandle handle) noexcept {

            auto entity = gameWorld->find<THandle>(handle);

            if (!entity) {
                logger_.warn("Entity was not found");
                return;
            }

            const auto* glfw = entity->get<GLFWWindowHandleComponent<THandle>>();
            if (!glfw) {
                logger_.error("Entity does not have GLFWWindowHandleComponent");
                assert(false && "Entity does not have GLFWWindowHandleComponent");
                return;
            }

            auto* wuptrComponent = entity->get<GLFWWindowUserPointerComponent<THandle>>();
            if (!wuptrComponent) {
                logger_.error("Entity does not have GLFWWindowUserPointerComponent");
                assert(false && "Entity does not have GLFWWindowUserPointerComponent");
                return;
            }
            auto* wuptr =  &wuptrComponent->userPointer;

            glfwSetWindowUserPointer(glfw->handle, static_cast<void*>(wuptr));

            glfwSetFramebufferSizeCallback(
                glfw->handle,
                [] (GLFWwindow* nativeHandle, const int width, const int height) {
                const auto* ptr = static_cast<GLFWWindowUserPointer<THandle>*>(glfwGetWindowUserPointer(nativeHandle));

                if (ptr && ptr->gameWorld) {
                    ptr->gameWorld->commandBuffer<EngineCommandBuffer>().add<WindowResizeCommand<THandle>>(
                        ptr->windowHandle,
                        WindowSize(width, height)
                    );
                }
            });
        }


        void swapBuffer(UpdateContext& updateContext, const SwapBuffersCommand<THandle>& cmd) noexcept {

            const auto entity = updateContext.find(cmd.windowHandle);

            if (!entity) {
                logger_.warn("Entity was not found");
                return;
            }

            const auto* glfw = entity->get<GLFWWindowHandleComponent<THandle>>();

            if (!glfw) {
                logger_.error("Entity does not have GLFWWindowHandleComponent");
                assert(false && "Entity does not have GLFWWindowHandleComponent");
                return;
            }

            assert((updateContext.session().state<GameState>() != GameState::Booting) &&
                "GLFWSwapBuffersSystem should not be running during boot");
            assert(glfw->handle && "GLFWWindowComponent has no native handle");
            glfwSwapBuffers(glfw->handle);
        }


        bool createWindows(UpdateContext& updateContext) noexcept {
            if (windowCreateCommands_.empty()) {
                return false;
            }
            for (const auto& windowCreateCommand  : windowCreateCommands_) {
                const bool isContextAvailable = createWindow(updateContext, windowCreateCommand);
                if (!isContextAvailable) {
                    logger_.error("Failed to create window");
                    assert(false && "Failed to create window");
                }
            }

            windowCreateCommands_.clear();
            return true;
        }

        void resizeWindows(UpdateContext& updateContext) noexcept {

            if (pendingResizeCommands_.empty()) {
                return;
            }

            for (const auto& [windowHandle, windowSize]: pendingResizeCommands_) {

                if (!windowHandle.isValid()) {
                    continue;
                }

                auto entity = updateContext.find(windowHandle);

                if (entity) {
                    if (auto* wc = entity->get<WindowComponent<THandle>>()) {
                        wc->size = windowSize;
                    }
                }

            }

            pendingResizeCommands_.clear();
        }

        void swapBuffers(UpdateContext& updateContext) noexcept {
            if (pendingBufferSwaps_.empty()) {
                return;
            }
            for (const auto& swapBufferCommand : pendingBufferSwaps_) {
                swapBuffer(updateContext, swapBufferCommand);
            }
            pendingBufferSwaps_.clear();
        }

        void pollEvents(UpdateContext& updateContext) noexcept {
            if (!pollEvents_) {
                return;
            }

            glfwPollEvents();
            pollEvents_ = false;
        }

        void closeWindows(UpdateContext& updateContext) noexcept {
            if (pendingCloseCommands_.empty()) {
                return;
            }

            for (const auto& cmd : pendingCloseCommands_) {
                auto entity = updateContext.find(cmd.windowHandle);

                if (!entity) {
                    logger_.warn("Entity was not found");
                    continue;
                }

                const auto* glfw = entity->get<GLFWWindowHandleComponent<THandle>>();
                if (!glfw) {
                    logger_.warn("Entity does not have GLFWWindowHandleComponent");
                    continue;
                }

                glfwDestroyWindow(glfw->handle);
                bool destroyed = gameWorld_->destroy<THandle>(cmd.windowHandle);
                assert(destroyed && "Failed to destroy entity");
            }

            pendingCloseCommands_.clear();
        }


        void shutdown(UpdateContext& updateContext) noexcept {

            glfwTerminate();

            updateContext.queueCommand<TCommandBuffer, StateCommand<GameState>>(
               StateTransitionRequest<GameState>(
                   updateContext.session().state<GameState>(),
                   GameStateTransitionId::ShutdownRequest
               )
           );


        }


        public:


        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = ManagerRole;


        /**
         * @brief Processes queued platform/window work for the current frame.
         *
         * @param updateContext Frame-local update context.
         */
        void flush(UpdateContext& updateContext)  noexcept {

            if (shouldShutdown_) {
                shutdown(updateContext);
                return;
            }

            initPlatform(updateContext);
            pollEvents(updateContext);
            const bool isContextAvailable = createWindows(updateContext);

            if (!openGLLoaded_ && isContextAvailable) {
                openGLLoaded_ = initOpenGL(updateContext);
            }

            resizeWindows(updateContext);
            closeWindows(updateContext);
            swapBuffers(updateContext);
        }

        bool submit(const PollEventsCommand& command) noexcept {
            pollEvents_ = true;
            return true;
        }

        bool submit(const PlatformInitCommand& command) noexcept {
            assert(!initialized_ && "Application was already initialized.");
            shouldInit_ = true;
            return true;
        }

        bool submit(const WindowCreateCommand<THandle>& command)  noexcept {
            windowCreateCommands_.push_back(command);
            return true;
        }

        bool submit(const SwapBuffersCommand<THandle>& command)  noexcept {
            pendingBufferSwaps_.push_back(command);
            return true;
        }

        bool submit(const WindowResizeCommand<THandle>& command)  noexcept {
            const auto idx = command.windowHandle.entityId;

            if (pendingResizeCommands_.size() <= idx) {
                pendingResizeCommands_.resize(idx + 1);
            }

            pendingResizeCommands_[idx] = command;
            return true;
        }

        bool submit(const WindowCloseCommand<THandle>& command)  noexcept {
            pendingCloseCommands_.push_back(command);
            return true;
        }

        bool submit(const ShutdownCommand& command)  noexcept {
            shouldShutdown_ = true;
            return true;
        }

        /**
         * @brief Registers this manager as handler for supported platform/window commands.
         *
         * @param gameWorld Runtime world used for command-handler registration.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) noexcept {

            gameWorld_ = &gameWorld;

            gameWorld.registerCommandHandler<
                WindowCreateCommand<THandle>,
                PlatformInitCommand,
                WindowResizeCommand<THandle>,
                SwapBuffersCommand<THandle>,
                PollEventsCommand,
                WindowCloseCommand<THandle>,
                ShutdownCommand
            >(*this);
        }
    };


}

