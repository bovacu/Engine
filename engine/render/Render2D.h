#pragma once

#ifndef RENDER_2D_H
#define RENDER_2D_H

#include <engine/render/camera/OrthographicCamera.h>
#include <engine/render/elements/Texture.h>
#include <glm/glm.hpp>
#include <engine/render/shapes/Shape.h>
#include <engine/render/elements/TextureRegion.h>
#include <engine/render/camera/Camera.h>

namespace engine {

    class Render2D {
        /// This is just for the debugging, in release there's not such data and won't even be compiled
    #if defined(ENGINE_DEBUG)
        public:
            struct Statistics {
                uint32_t drawCalls = 0;
                uint32_t quadCount = 0;

                [[nodiscard]] uint32_t getTotalVertexCount() const { return quadCount * 4; }
                [[nodiscard]] uint32_t getTotalIndexCount()  const { return quadCount * 6; }
            };
    #endif

        private:
            static void init();
            static void shutdown();

            static void beginDraw(const OrthographicCamera& camera);
            static void beginDraw(const Camera& _camera, const glm::mat4& _transform);
            static void endDraw();
            static void flush();

            static void drawRect(const glm::mat4& _transform, const Color& _color);
            static void drawTexture(const glm::mat4& _transform, const Texture2DPtr& _texture, float _rotation, const Color& _tintColor = Color::Transparent);
            static void drawTexture(const glm::mat4& _transform, const TextureRegionPtr& _textureRegion, float _rotation, const Color& _tintColor = Color::Transparent);

            // Primitives
            static void drawRect(const Vec2f& _position, const Vec2f& _size, const Color& _color);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const Texture2DPtr& _texture, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const TextureRegionPtr& _textureRegion, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);
            static void drawRotatedRect(const Vec2f& _position, const Vec2f& _size, float _rotation = 0.0f, const Color& _color = Color::Transparent);
            static void drawRotatedTexture(const Vec2f& _position, const Vec2f& _size, float _rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawLine(const Vec2f& _p0, const Vec2f& _p1, const Color& _color, float _thickness = 1.0f);
            static void drawShape(Shape& _shape, const Color& _color, float _thickness = 1.0f);

            #if defined(ENGINE_DEBUG)
                static void resetStats();
                static Statistics getStats();
            #endif

            static void flushAndReset();
            static void drawRect(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
            static void drawTexture(const glm::vec3& _position, const glm::vec2& _size, const Texture2DPtr& _texture, float _rotation = 0.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));
            static void drawTexture(const glm::vec3& _position, const glm::vec2& _size, const TextureRegionPtr& _textureRegion, float _rotation = 0.0f, const glm::vec4& _tintColor = glm::vec4(0.0f));
            static void drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color);
            static void drawRotatedTexture(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const Texture2DPtr& _texture, float _tilingFactor = 1.0f, const glm::vec4& _tintColor = glm::vec4(1.0f));

        public:
            friend class Renderer;
    };

}

#endif //ENGINE_RENDER2D_H
