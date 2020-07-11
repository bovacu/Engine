#include "pch.h"
#include "FrameBuffer.h"

#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLFrameBuffer.h>

namespace engine {


    FrameBufferPtr FrameBuffer::create(const FrameBufferSpecification& _specs) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLFrameBuffer>(_specs);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}