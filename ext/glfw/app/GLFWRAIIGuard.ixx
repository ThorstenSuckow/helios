module;

export module helios.ext.glfw.app.GLFWRAIIGuard;


export namespace helios::ext::glfw::app {

    class GLFWRAIIGuard {

    public:
        GLFWRAIIGuard();

        ~GLFWRAIIGuard();

        GLFWRAIIGuard(const GLFWRAIIGuard&) = delete;

        GLFWRAIIGuard operator=(const GLFWRAIIGuard&) = delete;
    };

}


