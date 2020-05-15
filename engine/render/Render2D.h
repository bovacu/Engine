#pragma once

#ifndef RENDER_2D_H
#define RENDER_2D_H

#include <engine/render/camera/OrthographicCamera.h>
#include <engine/render/elements/Texture.h>
#include <glm/glm.hpp>
#include <engine/gameObject/GameObject.h>
#include <engine/render/shapes/Shape.h>

namespace engine {

    enum RenderingType { QUADS, PIXEL };

    class Render2D {
        // Stats
        public:
            struct Statistics {
                uint32_t drawCalls = 0;
                uint32_t quadCount = 0;

                uint32_t getTotalVertexCount() { return quadCount * 4; }
                uint32_t getTotalIndexCount() { return quadCount * 6; }
            };

            static RenderingType renderingType;

        public:
            static void init();
            static void shutdown();

            static void beginRender(const OrthographicCamera& camera, const RenderingType& _type = RenderingType::QUADS);
            static void endRender();
            static void flush();

            // Primitives
            static void drawRect(const Vec2f& _position, const Size& _size, const Color& _color);
            static void drawTextureRect(const Vec2f& _position, const Size& _size, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRotatedRect(const Vec2f& _position, const Size& _size, float rotation, const Color& _color);
            static void drawRotatedTextureRect(const Vec2f& _position, const Size& _size, float rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void draw(const GameObjectPtr& _gameObject, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void draw(const Shape& _shape);
            static void drawPixel(const Vec2f& _position, const Color& _color);

            static void resetStats();
            static Statistics getStats();

        private:
            static void flushAndReset();
            static void drawRotated(const SpritePtr & _sprite, float _rotation, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRect(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
            static void drawTextureRect(const glm::vec3& _position, const glm::vec2& _size, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const glm::vec4& _color);
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawPixel(const glm::vec3& _position, const glm::vec4& _color);
    };

}

#endif //ENGINE_RENDER2D_H
