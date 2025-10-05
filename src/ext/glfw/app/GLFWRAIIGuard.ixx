module;

export module helios.ext.glfw.app.GLFWRAIIGuard;

export namespace helios::ext::glfw::app {

    /**
     * A RAII Guard for GLFW.
     *
     * This class makes sure that GLFW is properly initialized when an instance is created
     * and terminated when this instance is destroyed.
     *
     * It should be called at the begin of the implementing `main()`-function.
     *
     * @see https://en.cppreference.com/w/cpp/language/raii.html
     */
    class GLFWRAIIGuard {

    public:
        /**
         * Initializes the glfw library by calling `glfwInit()`.
         *
         * @throws std::runtime_error if initialization fails
         *
         * @see https://www.glfw.org/docs/latest/group__init.html#ga317aac130a235ab08c6db0834907d85e
         */
        GLFWRAIIGuard();


        /**
         * Terminates the glfw library by calling `glfwTerminate()`.
         *
         * @see https://www.glfw.org/docs/latest/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
         */
        ~GLFWRAIIGuard();

        /**
         * No copy or move.
         */
        GLFWRAIIGuard(const GLFWRAIIGuard&) = delete;
        GLFWRAIIGuard& operator=(const GLFWRAIIGuard&) = delete;
        GLFWRAIIGuard(const GLFWRAIIGuard&&) = delete;
        GLFWRAIIGuard& operator=(const GLFWRAIIGuard&&) = delete;
    };

}


