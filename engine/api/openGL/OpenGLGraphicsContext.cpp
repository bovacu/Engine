#include "pch.h"
#include "OpenGLGraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {

    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* _windowHandle) : windowHandle(_windowHandle) {  }

    void OpenGLGraphicsContext::init() {
        ENGINE_CORE_ASSERT(this->windowHandle, "Window handle is null!")

        /// First, initialization to the Window.
        glfwMakeContextCurrent(this->windowHandle);

        /// Checking if there are errors.
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!")

        #pragma region DebugInfoAndChecks
            #ifdef ENGINE_DEBUG
                LOG_INFO_CORE("OpenGL Info:");
                LOG_INFO_CORE("  Vendor: {0}", glGetString(GL_VENDOR));
                LOG_INFO_CORE("  Renderer: {0}", glGetString(GL_RENDERER));
                LOG_INFO_CORE("  Version: {0}", glGetString(GL_VERSION));
            #endif

            #ifdef ENGINE_ENABLE_ASSERTS
                    int versionMajor;
                    int versionMinor;
                    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
                    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

                    ENGINE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Engine requires at least OpenGL version 4.5!")
            #endif
        #pragma endregion
    }

    void OpenGLGraphicsContext::swapBuffers() {
        /// This updates the window and allows rendering.
        glfwSwapBuffers(this->windowHandle);
    }

}