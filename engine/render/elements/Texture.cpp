#include "pch.h"
#include "Texture.h"

#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLTexture.h>

namespace engine {

    Texture2DPtr Texture2D::create(uint32_t _width, uint32_t _height, bool _useAlpha) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLTexture>(_width, _height, _useAlpha);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Texture2DPtr Texture2D::create(const std::string& _path) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLTexture>(_path);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
