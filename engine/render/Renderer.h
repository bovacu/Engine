#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <engine/render/RenderAPI.h>
#include <engine/render/elements/Shader.h>
//#include <engine/render/camera/OrthographicCamera.h>
#include <engine/render/elements/Texture.h>
#include <engine/render/elements/TextureRegion.h>
#include <engine/render/shapes/Shape.h>
#include <engine/render/camera/Camera.h>

namespace engine {

    class Renderer {
        friend class Render2D;

        private:
            struct SceneData {
                glm::mat4 viewProjectionMatrix;
            };

        static std::unique_ptr<SceneData> sceneData;
        static RenderAPIPtr renderAPI;

        private:
            static RenderAPIPtr& getRendererAPI() { return renderAPI; }

        public:
            static void init();
            static void shutdown();

            static void onWindowResize(uint32_t _width, uint32_t _height);

//            static void beginDrawCall(OrthographicCamera& _camera);
            static void beginDrawCall(const Camera& _camera, glm::mat4& _transform);
            static void drawLine(const Vec2f& _p0, const Vec2f& _p1, const Color& _color, float _thickness = 1.0f);

            static void drawRectangle(const Vec2f& _position, const Vec2f& _size, const Color& _color, float _rotation = 0.f, const Color& _tintColor = Color::Transparent);
            static void drawRectangle(const glm::mat4& _transform, const Color& _color);

            static void drawShape(Shape& _shape, const Color& _color, float _thickness = 1.0f);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const Texture2DPtr& _texture, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);
            static void drawTexture(const Vec2f& _position, const Vec2f& _size, const TextureRegionPtr& _textureRegion, float _rotation = 0.0f, const Color& _tintColor = Color::Transparent);

            static void endDrawCall();

            static void submit(const ShaderPtr& _shader, const VertexArrayPtr& _vertexArray, const glm::mat4& _transform = glm::mat4(1.0f));
            static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }

            static void setClearColor(const Color& _color);
            static void clear();
    };

}

#endif // RENDERER_H
