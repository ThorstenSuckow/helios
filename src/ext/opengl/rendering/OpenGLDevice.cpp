module;

#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

module helios.ext.opengl.rendering.OpenGLDevice;

import helios.ext.opengl.rendering.model.OpenGLMesh;

using namespace helios::util;
using namespace helios::ext::opengl::rendering::model;


namespace helios::ext::opengl::rendering {

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
            if (const auto shader_ptr = rc->shader().lock()) {
                logger_.info("activating shader...");
                shader_ptr->use();
                shader_ptr->applyUniformValues(rc->objectUniformValues());
                shader_ptr->applyUniformValues(rc->materialUniformValues());
            }
            if (const auto mesh_ptr = rc->mesh().lock()) {
                const auto mesh =  std::dynamic_pointer_cast<const OpenGLMesh>(mesh_ptr);
                logger_.info(std::format("Binding vao {0}", mesh->vao()));
                glBindVertexArray(mesh->vao());

                glDrawElements(GL_LINE_LOOP,
                    mesh_ptr->indexCount() ,
                    GL_UNSIGNED_INT, nullptr
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
