#include "pch.h"
#include "Buffer.h"
#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLBuffer.h>

namespace engine {

    VertexBufferPtr VertexBuffer::create(uint32_t size) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    VertexBufferPtr VertexBuffer::create(float* vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBufferPtr IndexBuffer::create(uint32_t* indices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}