/**
 * @file GLFWPlatformManager.ixx
 * @brief GLFW-backed platform manager handling runtime init, window lifecycle, and frame service commands.
 */
module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <ostream>
#include <ranges>
#include <vector>

export module helios.platform.glfw.GLFWPlatformManager;


import helios.util.log;
import helios.ecs.types.EntityHandle;
import helios.core.types;
import helios.state.Bindings;


import helios.state.commands;
import helios.state.types;
import helios.gameplay.gamestate.types;

import helios.engine.common.tags.ManagerRole;

import helios.runtime.world;

import helios.platform.environment.commands;
import helios.platform.lifecycle.commands;
import helios.platform.environment.components;
import helios.platform.environment.types;

import helios.platform.window.commands;
import helios.platform.window.components;
import helios.platform.window.types;

import helios.platform.glfw.components;
import helios.platform.glfw.types;

import helios.ecs.concepts;
import helios.engine.common.concepts;
import helios.runtime.messaging.command;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::common::tags;
using namespace helios::platform::environment::commands;
using namespace helios::platform::lifecycle::commands;
using namespace helios::platform::environment::types;
using namespace helios::platform::environment::components;
using namespace helios::platform::window::commands;
using namespace helios::platform::window::types;
using namespace helios::platform::window::components;
using namespace helios::platform::glfw::components;
using namespace helios::platform::glfw::types;
using namespace helios::gameplay::gamestate::types;
using namespace helios::state::commands;
using namespace helios::state::types;
using namespace helios::ecs;
using namespace helios::ecs::concepts;
using namespace helios::engine::common::concepts;
using namespace helios::core::types;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::world;
using namespace helios::platform::window::concepts;

#define HELIOS_LOG_SCOPE "helios::platform::glfw::GLFWPlatformManager"
export namespace helios::platform::glfw {

    /**
     * @brief Concrete manager integrating GLFW with helios runtime/window command flow.
     *
     * The manager receives runtime and window commands via `submit(...)`, stores them
     * as pending work, and processes them in `flush(...)` in a deterministic order.
     *
     * @tparam THandle Window/entity handle type.
     * @tparam TStateCommandBuffer Command buffer used for follow-up state commands.
     * @tparam TPlatformCommandBuffer Command buffer used by GLFW callbacks for platform commands.
     */
    template<typename THandle, typename TStateCommandBuffer = NullCommandBuffer, typename TPlatformCommandBuffer = NullCommandBuffer>
    requires IsWindowHandle<THandle>
            && IsCommandBufferLike<TStateCommandBuffer>
            && IsCommandBufferLike<TPlatformCommandBuffer>
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

        /**
         * @brief Initializes GLFW and transitions runtime/session from booting to boot request.
         *
         * @param updateContext Frame-local update context.
         */
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

            updateContext.queueCommand<TStateCommandBuffer, StateCommand<GameState>>(
                StateTransitionRequest<GameState>(
                    updateContext.session().state<GameState>(),
                    GameStateTransitionId::BootRequest
                )
            );

            shouldInit_ = false;
        }


        /**
         * @brief Creates a native GLFW window and binds it to the requested window entity.
         *
         * @param updateContext Frame-local update context.
         * @param cmd Window creation command containing target handle and config.
         *
         * @return `true` if the window was created and bound successfully; otherwise `false`.
         */
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

            assert(window->template has<WindowCreateRequestComponent<THandle>>() && "Expected entity to have WindowCreateRequestComponent");
            window->template remove<WindowCreateRequestComponent<THandle>>();
            assert(!window->template has<WindowCreateRequestComponent<THandle>>() && "Expected entity to not have WindowCreateRequestComponent");
            assert(!window->template has<WindowComponent<THandle>>() && "Expected entity to not have WindowComponent");
            window->template add<WindowComponent<THandle>>(
                std::move(cfg.title),
                WindowSize{cfg.size.width, cfg.size.height},
                cfg.aspectRatioNumer,
                cfg.aspectRatioDenom
            );
            window->template add<GLFWWindowHandleComponent<THandle>>(nativeHandle);

            removeCurrentContext(updateContext);

            glfwMakeContextCurrent(nativeHandle);

            window->template add<CurrentContextComponent<THandle>>();

            window->template add<WindowShownComponent<THandle>>();
            window->template add<GLFWWindowUserPointerComponent<THandle>>(GLFWWindowUserPointer<THandle>(cmd.windowHandle, gameWorld_));

            installResizeListener(gameWorld_, cmd.windowHandle);

            return true;
        }

        /**
         * @brief Loads OpenGL function pointers for the current context.
         *
         * @param updateContext Frame-local update context.
         *
         * @return `true` if OpenGL was loaded successfully; otherwise `false`.
         */
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

        /**
         * @brief Removes `CurrentContextComponent` from all windows currently marked as active context.
         *
         * @param updateContext Frame-local update context.
         */
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

        /**
         * @brief Installs GLFW user-pointer data and framebuffer resize callback for a window.
         *
         * @param gameWorld Owning game world used by callback command submission.
         * @param handle Window handle for which the listener is installed.
         */
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
                    ptr->gameWorld->commandBuffer<TPlatformCommandBuffer>().add<WindowResizeCommand<THandle>>(
                        ptr->windowHandle,
                        WindowSize(width, height)
                    );
                }
            });
        }


        /**
         * @brief Executes a single swap-buffers command.
         *
         * @param updateContext Frame-local update context.
         * @param cmd Swap-buffers command.
         */
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


        /**
         * @brief Creates all windows queued during command submission.
         *
         * @param updateContext Frame-local update context.
         *
         * @return `true` if at least one window was created in this flush; otherwise `false`.
         */
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

        /**
         * @brief Applies queued resize commands to window components.
         *
         * @param updateContext Frame-local update context.
         */
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

        /**
         * @brief Processes all queued swap-buffers commands.
         *
         * @param updateContext Frame-local update context.
         */
        void swapBuffers(UpdateContext& updateContext) noexcept {
            if (pendingBufferSwaps_.empty()) {
                return;
            }
            for (const auto& swapBufferCommand : pendingBufferSwaps_) {
                swapBuffer(updateContext, swapBufferCommand);
            }
            pendingBufferSwaps_.clear();
        }

        /**
         * @brief Polls GLFW events if requested by a poll-events command.
         *
         * @param updateContext Frame-local update context.
         */
        void pollEvents(UpdateContext& updateContext) noexcept {
            if (!pollEvents_) {
                return;
            }

            glfwPollEvents();
            pollEvents_ = false;
        }

        /**
         * @brief Destroys all windows queued for closing.
         *
         * @param updateContext Frame-local update context.
         */
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


        /**
         * @brief Terminates GLFW and queues a shutdown state transition request.
         *
         * @param updateContext Frame-local update context.
         */
        void shutdown(UpdateContext& updateContext) noexcept {

            glfwTerminate();

            updateContext.queueCommand<TStateCommandBuffer, StateCommand<GameState>>(
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

        /**
         * @brief Marks that platform events should be polled in the next `flush(...)`.
         *
         * @param command Poll-events marker command.
         *
         * @return `true` when the command was accepted.
         */
        bool submit(const PollEventsCommand& command) noexcept {
            pollEvents_ = true;
            return true;
        }

        /**
         * @brief Marks platform initialization for execution during the next `flush(...)`.
         *
         * @param command Platform-init marker command.
         *
         * @return `true` when the command was accepted.
         */
        bool submit(const PlatformInitCommand& command) noexcept {
            assert(!initialized_ && "Application was already initialized.");
            shouldInit_ = true;
            return true;
        }

        /**
         * @brief Queues a window creation request.
         *
         * @param command Window creation command.
         *
         * @return `true` when the command was queued.
         */
        bool submit(const WindowCreateCommand<THandle>& command)  noexcept {
            windowCreateCommands_.push_back(command);
            return true;
        }

        /**
         * @brief Queues a buffer-swap request for a specific window.
         *
         * @param command Swap-buffers command.
         *
         * @return `true` when the command was queued.
         */
        bool submit(const SwapBuffersCommand<THandle>& command)  noexcept {
            pendingBufferSwaps_.push_back(command);
            return true;
        }

        /**
         * @brief Stores the latest resize request per window entity index.
         *
         * @param command Window resize command.
         *
         * @return `true` when the command was stored.
         */
        bool submit(const WindowResizeCommand<THandle>& command)  noexcept {
            const auto idx = command.windowHandle.entityId;

            if (pendingResizeCommands_.size() <= idx) {
                pendingResizeCommands_.resize(idx + 1);
            }

            pendingResizeCommands_[idx] = command;
            return true;
        }

        /**
         * @brief Queues a window-close request.
         *
         * @param command Window-close command.
         *
         * @return `true` when the command was queued.
         */
        bool submit(const WindowCloseCommand<THandle>& command)  noexcept {
            pendingCloseCommands_.push_back(command);
            return true;
        }

        /**
         * @brief Marks platform shutdown for execution during the next `flush(...)`.
         *
         * @param command Shutdown marker command.
         *
         * @return `true` when the command was accepted.
         */
        bool submit(const ShutdownCommand& command)  noexcept {
            shouldShutdown_ = true;
            return true;
        }

        /**
         * @brief Registers this manager as handler for supported platform/window commands.
         *
         * @param gameWorld Runtime world used for command-handler registration.
         */
        void init(helios::runtime::world::GameWorld& gameWorld) noexcept {

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

