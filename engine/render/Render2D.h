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

            static void beginDraw(const OrthographicCamera& camera, const RenderingType& _type = RenderingType::QUADS);
            static void endDraw();
            static void flush();

            // Primitives
            static void drawRect(const Vec2f& _position, const Vec2f& _size, const Color& _color);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const Texture2DPtr& _texture, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const TextureRegionPtr& _textureRegion, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);
            static void drawRotatedRect(const Vec2f& _position, const Vec2f& _size, float _rotation = 0.0f, const Color& _color = Color::Transparent);
            static void drawRotatedTexture(const Vec2f& _position, const Vec2f& _size, float _rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void draw(const GameObjectPtr& _gameObject, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));

            static void resetStats();
            static Statistics getStats();

        private:
            static void flushAndReset();
            static void drawRotated(const SpritePtr & _sprite, float _rotation, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawRect(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
            static void drawTexture(const glm::vec3& _position, const glm::vec2& _size, const Texture2DPtr& _texture, float _rotation = 0.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawTexture(const glm::vec3& _position, const glm::vec2& _size, const TextureRegionPtr& _textureRegion, float _rotation = 0.0f, const glm::vec4& _tintColor = glm::vec4(0.0f));
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color);
            static void drawRotatedTexture(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
    };

}

#endif //ENGINE_RENDER2D_H
