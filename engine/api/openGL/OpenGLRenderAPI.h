#pragma once

#ifndef OPENGL_RENDER_API_H
#define OPENGL_RENDER_API_H

#include "engine/render/RenderAPI.h"

namespace engine {

    class OpenGLRenderAPI : public RenderAPI {
        public:
            virtual void init() override;
            virtual void setViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) override;

            virtual void setClearColor(const glm::vec4& _color) override;
            virtual void clear() override;

            virtual void drawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount = 0) override;
    };

}

#endif //OPENGL_RENDER_API_H
