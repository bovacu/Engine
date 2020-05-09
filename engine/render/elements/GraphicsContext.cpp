#include "GraphicsContext.h"

#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLGraphicsContext.h>

namespace engine {

    GraphicsContextPtr GraphicsContext::create(void* window) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}