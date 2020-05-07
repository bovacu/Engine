#pragma once

#ifndef RENDER_2D_H
#define RENDER_2D_H

#include "engine/render/camera/OrthographicCamera.h"
#include "engine/render/elements/Texture.h"
#include <glm/glm.hpp>

namespace engine {

    class Render2D {
        // Stats
        public:
            struct Statistics {
                uint32_t drawCalls = 0;
                uint32_t quadCount = 0;

                uint32_t getTotalVertexCount() { return quadCount * 4; }
                uint32_t getTotalIndexCount() { return quadCount * 6; }
            };

        public:
            static void init();
            static void shutdown();

            static void beginRender(const OrthographicCamera& camera);
            static void endRender();
            static void flush();

            // Primitives
            static void drawRect(const Vec2f& _position, const Size& _size, const Color& _color);
            static void drawRect(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
            static void drawRect(const Vec2f& _position, const Size& _size, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRect(const glm::vec3& _position, const glm::vec2& _size, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));

            static void drawRotatedRect(const Vec2f& _position, const Size& _size, float rotation, const Color& _color);
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const glm::vec4& _color);
            static void drawRotatedRect(const Vec2f& _position, const Size& _size, float rotation, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));

            static void resetStats();
            static Statistics getStats();

        private:
            static void flushAndReset();
        };

}

#endif //ENGINE_RENDER2D_H
