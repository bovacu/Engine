#pragma once

#ifndef RENDER_API_H
#define RENDER_API_H

#include "engine/render/elements/VertexArray.h"
#include <glm/glm.hpp>

namespace engine {

    class RenderAPI {
        public:
            enum class API {
                None = 0, OpenGL = 1
            };

        private:
            static API _API;

        public:
            virtual void init() = 0;
            virtual void setViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) = 0;
            virtual void setClearColor(const glm::vec4& _color) = 0;
            virtual void clear() = 0;

            virtual void drawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount = 0) = 0;

            static API getAPI() { return _API; }
            static std::unique_ptr<RenderAPI> create();

    };

}

#endif // RENDER_API_H
