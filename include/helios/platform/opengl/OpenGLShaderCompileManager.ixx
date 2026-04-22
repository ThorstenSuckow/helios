/**
 * @file OpenGLShaderCompileManager.ixx
 * @brief OpenGL manager that compiles shader sources into native OpenGL programs.
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


export module helios.platform.opengl.OpenGLShaderCompileManager;

import helios.util.log;
import helios.util.io;
import helios.runtime.world.tags;
import helios.runtime.world;
import helios.rendering.shader.commands;
import helios.rendering.shader.components;
import helios.rendering.shader.ShaderEntity;
import helios.rendering.shader.types;
import helios.rendering.shader.concepts;
import helios.rendering.shader.commands;
import helios.platform.opengl.components.OpenGLShaderComponent;

import helios.runtime.world.EngineWorld;
import helios.runtime.messaging.command.concepts;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.CommandHandlerRegistry;
import helios.runtime.concepts;

using namespace helios::runtime::world;
using namespace helios::util::log;
using namespace helios::util::io;
using namespace helios::runtime::tags;
using namespace helios::rendering::shader::commands;
using namespace helios::rendering::shader::components;
using namespace helios::rendering::shader;
using namespace helios::rendering::shader::types;
using namespace helios::rendering::shader::concepts;
using namespace helios::rendering::shader::commands;
using namespace helios::platform::opengl::components;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::runtime::messaging::command;
#define HELIOS_LOG_SCOPE "helios::platform::opengl::OpenGLShaderCompileManager"
export namespace helios::platform::opengl {

    /**
     * @brief Manager that consumes shader compile commands and performs OpenGL compilation/linking.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCommandBuffer Command buffer type for optional follow-up commands.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsShaderHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class OpenGLShaderCompileManager {

        BasicStringFileReader stringFileReader_;

        RenderResourceWorld& renderResourceWorld_;

        std::vector<ShaderHandle> shaderHandles_;

        std::string vertexShaderSource_;

        std::string fragmentShaderSource_;


        inline static const Logger& logger_ = LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Loads the specified vertex and fragment shader.
         *
         * @return true if loading succeeded, otherwise false.
         *
         * @throws if loading the specified files failed.
         */
        bool load(
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath,
            std::string& vertexShaderSource,
            std::string& fragmentShaderSource
        ) noexcept {

            const bool frag = stringFileReader_.readInto(fragmentShaderPath, fragmentShaderSource);
            const bool vert = stringFileReader_.readInto(vertexShaderPath, vertexShaderSource);

            assert(frag && vert && "Could not load shader files");

            return frag && vert;
        }


        /**
         * @brief Compiles the vertex and fragment shader represented by this instance.
         *
         * @return true if compilation succeeded, otherwise false.
         *
         * @throws if compilation failed.
         */
        bool compile(ShaderEntity shader) noexcept {

            using Handle = typename ShaderEntity::Handle_type;

            if (shader.template get<OpenGLShaderComponent<Handle>>()) {
                logger_.error("Shader already has a ShaderComponent");
                assert(false && "Shader already has a ShaderComponent");
                return false;
            }

            auto* shaderSourceComponent = shader.template get<ShaderSourceComponent<Handle>>();


            const bool loaded = load(
                shaderSourceComponent->vertexShaderPath,
                shaderSourceComponent->fragmentShaderPath,
                vertexShaderSource_, fragmentShaderSource_);

            if (!loaded) {
                logger_.error("Could not load shader files");
                assert(false && "Could not load shader files");
                return false;
            }

            const GLchar* vertexSrc = vertexShaderSource_.c_str();
            const GLchar* fragmentSrc = fragmentShaderSource_.c_str();

            const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
            glCompileShader(vertexShader);

            const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
            glCompileShader(fragmentShader);

            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
                logger_.error("Vertex Shader Compilation failed.");
                assert(false && "Vertex Shader Compilation failed.");
                return false;
            }

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
                logger_.error("Fragment Shader Compilation failed.");
                assert(false && "Fragment Shader Compilation failed.");
                return false;
            }

            auto progId = glCreateProgram();

            glAttachShader(progId, vertexShader);
            glAttachShader(progId, fragmentShader);
            glLinkProgram(progId);

            glGetProgramiv(progId, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(progId, 512, nullptr, infoLog);
                logger_.error("Program linking failed.");
                assert(false && "Program linking failed.");
                return false;
            }

            auto& shaderComponent = shader.add<OpenGLShaderComponent<THandle>>();
            shaderComponent.programId = progId;

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            vertexShaderSource_.clear();
            fragmentShaderSource_.clear();

            return true;
        }

        public:

        /**
         * @brief Constructs the manager with access to render-resource world storage.
         *
         * @param renderResourceWorld Render-resource world used to resolve shader entities.
         */
        explicit OpenGLShaderCompileManager(RenderResourceWorld& renderResourceWorld)
        : 
        renderResourceWorld_(renderResourceWorld)
        { }

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = ManagerRole;


        /**
         * @brief Compiles all queued shaders and clears processed command data.
         *
         * @param updateContext Frame-local update context.
         */
        void flush(UpdateContext& updateContext)  noexcept {

            if (shaderHandles_.empty()) {
                return;
            }

            for (const auto& sourceHandle : shaderHandles_) {
                auto shaderEntity = renderResourceWorld_.findEntity<THandle>(sourceHandle);

                if (!shaderEntity) {
                    logger_.error("Could not find shader source");
                    assert(false && "Could not find shader source");
                    continue;
                }

                if (!compile(*shaderEntity)) {
                    logger_.error("Could not compile shader");
                } else {
                    shaderEntity->template remove<ShaderSourceComponent<THandle>>();
                }
            }

            shaderHandles_.clear();
        }

        bool submit(const ShaderBatchCompileCommand<THandle>& command)  noexcept {
            for (const auto& shaderHandle : command.shaderHandles) {
                shaderHandles_.push_back(shaderHandle);
            }
            return true;
        }

        bool submit(const ShaderCompileCommand<THandle>& command)  noexcept {
            shaderHandles_.push_back(command.shaderHandle);
            return true;
        }

        /**
         * @brief Registers compile command handlers in the runtime world.
         *
         * @param commandHandlerRegistry Registry used for command-handler registration.
         */
        void init(helios::runtime::messaging::command::CommandHandlerRegistry& commandHandlerRegistry) noexcept {
            commandHandlerRegistry.registerHandler<ShaderCompileCommand<THandle>>(*this);
            commandHandlerRegistry.registerHandler<ShaderBatchCompileCommand<THandle>>(*this);
        }
    };


}