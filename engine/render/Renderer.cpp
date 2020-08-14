#include "pch.h"
#include "Renderer.h"
#include <engine/render/Render2D.h>

namespace engine {

    std::unique_ptr<Renderer::SceneData> Renderer::sceneData = std::make_unique<Renderer::SceneData>();
    RenderAPIPtr Renderer::renderAPI = RenderAPI::create();

    void Renderer::init() {
        renderAPI->init();
        Render2D::init();
    }
    void Renderer::shutdown() {
        Render2D::shutdown();
    }

    void Renderer::onWindowResize(uint32_t _width, uint32_t _height) {
        renderAPI->setViewport(0, 0, _width, _height);
    }

    void Renderer::setClearColor(const Color& _color) {
        renderAPI->setClearColor(_color);
    }
    void Renderer::clear() {
        renderAPI->clear();
    }

    void Renderer::beginDrawCall(OrthographicCamera& _camera) {
        Render2D::beginDraw(_camera);
    }
    void Renderer::beginDrawCall(const Camera& _camera, glm::mat4& _transform) {
        Render2D::beginDraw(_camera, _transform);
    }
    void Renderer::endDrawCall() {
        Render2D::endDraw();
    }
    void Renderer::submit(const ShaderPtr& _shader, const VertexArrayPtr& _vertexArray, const glm::mat4& _transform) {
        _shader->bind();
        _shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
        _shader->setMat4("u_Transform", _transform);

        _vertexArray->bind();
        renderAPI->drawIndexed(_vertexArray);
    }

    void Renderer::drawLine(const Vec2f& _p0, const Vec2f& _p1, const Color& _color, float _thickness) {
        Render2D::drawLine(_p0, _p1, _color, _thickness);
    }
    void Renderer::drawShape(Shape& _shape, const Color& _color, float _thickness) {
        Render2D::drawShape(_shape, _color, _thickness);
    }
    void Renderer::drawRectangle(const glm::mat4& _transform, const Color& _color) {
        Render2D::drawRect(_transform, _color);
    }
    void Renderer::drawRectangle(const Vec2f& _position, const Vec2f& _size, const Color& _color, float _rotation, const Color& _tintColor) {
        if(_rotation > 0)
            Render2D::drawRotatedRect(_position, _size, _rotation, _color);
        else
            Render2D::drawRect(_position, _size, _color);
    }
    void Renderer::drawTexture(const Vec2f& _position, const Vec2f& _size, const Texture2DPtr& _texture, float _rotation, const Color& _tintColor) {
        Render2D::drawTexture(_position, _size, _texture, _rotation, _tintColor);
    }
    void Renderer::drawTexture(const Vec2f& _position, const Vec2f& _size, const TextureRegionPtr& _textureRegion, float _rotation, const Color& _tintColor) {
        Render2D::drawTexture(_position, _size, _textureRegion, _rotation, _tintColor);
    }



}