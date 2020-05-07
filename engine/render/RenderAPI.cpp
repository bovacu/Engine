#include "pch.h"
#include "RenderAPI.h"

#include "engine/api/openGL/OpenGLRenderAPI.h"

namespace engine {

    RenderAPI::API RenderAPI::_API = RenderAPI::API::OpenGL;

    std::unique_ptr<RenderAPI> RenderAPI::create() {
        switch (_API) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_unique<OpenGLRenderAPI>();
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
