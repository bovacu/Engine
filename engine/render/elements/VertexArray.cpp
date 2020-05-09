#include "pch.h"
#include "VertexArray.h"

#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLVertexArray.h>

namespace engine {

    VertexArrayPtr VertexArray::create() {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}