#pragma once

#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include <engine/render/RenderAPI.h>

namespace engine {

    /// This class is needed as some of the functions that go from Renderer2D to Renderer needs a middle point to be
    /// correctly called. That happens with drawIndexed(...).
    class RenderMiddle {
        private:
            static RenderAPIPtr renderAPI;

        private:
            static void init() {
                renderAPI->init();
            }

            static void setViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) {
                renderAPI->setViewport(_x, _y, _width, _height);
            }

            static void setClearColor(const Color& _color) {
                renderAPI->setClearColor({_color.r, _color.g, _color.b, _color.a});
            }

            static void setClearColor(const glm::vec4& _color) {
                renderAPI->setClearColor(_color);
            }

            static void clear() {
                renderAPI->clear();
            }

            static void drawIndexed(const VertexArrayPtr& _vertexArray, uint32_t _count = 0) {
                renderAPI->drawIndexed(_vertexArray, _count);
            }

        public:
            friend class Renderer;
            friend class Render2D;
    };

}

#endif // RENDER_COMMAND_H
