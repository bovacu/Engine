#include "pch.h"
#include "OpenGLGraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {

    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* _windowHandle)
            : windowHandle(_windowHandle) { ENGINE_CORE_ASSERT(_windowHandle, "Window handle is null!") }

    void OpenGLGraphicsContext::init() {
        glfwMakeContextCurrent(this->windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!")

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
    }

    void OpenGLGraphicsContext::swapBuffers() {
        glfwSwapBuffers(this->windowHandle);
    }

}