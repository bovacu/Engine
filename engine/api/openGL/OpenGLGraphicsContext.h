#pragma once

#ifndef OPENGL_GRAPHICS_CONTEXT_H
#define OPENGL_GRAPHICS_CONTEXT_H

#include <engine/render/elements/GraphicsContext.h>

struct GLFWwindow;

namespace engine {

    class OpenGLGraphicsContext : public GraphicsContext {
        private:
            GLFWwindow*                     windowHandle = nullptr;

        public:
            explicit OpenGLGraphicsContext(GLFWwindow* _windowHandle);

            void init()             override;
            void swapBuffers()      override;
    };

}

#endif // OPENGL_GRAPHICS_CONTEXT_H
