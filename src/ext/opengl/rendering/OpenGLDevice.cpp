module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

module helios.ext.opengl.rendering.OpenGLDevice;

import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.model.config.PrimitiveType;


using namespace helios::util;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::rendering::model::config;


namespace helios::ext::opengl::rendering {

    GLenum OpenGLDevice::toOpenGL(const PrimitiveType primitiveType) const noexcept {
        switch (primitiveType) {
            case PrimitiveType::Points: return GL_POINTS;
            case PrimitiveType::Lines: return GL_LINES;
            case PrimitiveType::LineLoop: return GL_LINE_LOOP;
            case PrimitiveType::LineStrip: return GL_LINE_STRIP;
            case PrimitiveType::Triangles: return GL_TRIANGLES;
            case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
            case PrimitiveType::TriangleFan: return GL_TRIANGLE_FAN;
            default:
                logger_.warn("Failed to resolve primitive type, falling back to GL_TRIANGLES.");
                return GL_TRIANGLES;
        }

    }

    void OpenGLDevice::init() {

        if (initialized_) {
            return ;
        }
        const GLADloadfunc procAddressLoader = glfwGetProcAddress;
        const int gl_ver = gladLoadGL(procAddressLoader);

        if (gl_ver == 0) {
            logger_.error("Failed to load OpenGL");
            throw std::runtime_error("Failed to load OpenGL");
        }

        logger_.info(std::format("OpenGL {0}.{1} loaded",
            GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver))
        );

        initialized_ = true;
    };


    void OpenGLDevice::clear() const noexcept {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLDevice::setClearColor(const math::vec4f& color) const  noexcept {
        glClearColor(color[0], color[1], color[2], color[3]);
    }

    void OpenGLDevice::beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept {
        clear();
    };


    void OpenGLDevice::doRender(helios::rendering::RenderPass& renderPass) const noexcept {

        const auto& renderQueue = renderPass.renderQueue();

        logger_.info(std::format("Rendering {0} item(s)...", renderQueue.count()));

        for (auto& rc: renderQueue.renderCommands()) {

            if (const auto renderPrototype_ptr = rc->renderPrototype().lock()) {
                const auto& baseShader = renderPrototype_ptr->material().shader();
                const auto& baseMesh   = renderPrototype_ptr->mesh();

                const auto* shader = dynamic_cast<const OpenGLShader*>(&baseShader);
                if (!shader) {
                    logger_.error("Failed to cast shader to OpenGLShader.");
                    continue;
                }
                logger_.info("activating shader...");
                shader->use();
                shader->applyUniformValues(rc->objectUniformValues());
                shader->applyUniformValues(rc->materialUniformValues());

                const auto* mesh = dynamic_cast<const OpenGLMesh*>(&baseMesh);
                if (!mesh) {
                    logger_.error("Failed to cast mesh to OpenGLMesh.");
                    continue;
                }
                const auto [primitiveType] = mesh->meshConfig();
                logger_.info(std::format("Binding vao {0}", mesh->vao()));
                glBindVertexArray(mesh->vao());
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDrawElements(
                    toOpenGL(primitiveType),
                    mesh->indexCount() ,
                    GL_UNSIGNED_INT,
                    nullptr
                );
                glBindVertexArray(0);
            }
        }
    }


    void OpenGLDevice::endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept {

    };

    void OpenGLDevice::setViewport(const int x, const int y, const int width, const int height) const noexcept {
        glViewport(x, y, width, height);
    }



};
