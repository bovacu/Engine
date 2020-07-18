#include "pch.h"
#include "Renderer.h"
#include <engine/render/Render2D.h>

namespace engine {

    std::unique_ptr<Renderer::SceneData> Renderer::sceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::init() {
        RenderCommand::init();
        Render2D::init();
    }
    void Renderer::shutdown() {
        Render2D::shutdown();
    }

    void Renderer::onWindowResize(uint32_t _width, uint32_t _height) {
        RenderCommand::setViewport(0, 0, _width, _height);
    }

    void Renderer::setClearColor(const Color& _color) {
        RenderCommand::setClearColor(_color);
    }
    void Renderer::clear() {
        RenderCommand::clear();
    }

    void Renderer::beginDrawCall(OrthographicCamera& _camera) {
        Render2D::beginDraw(_camera);
    }
    void Renderer::endDrawCall() {
        Render2D::endDraw();
    }
    void Renderer::submit(const ShaderPtr& _shader, const VertexArrayPtr& _vertexArray, const glm::mat4& _transform) {
        _shader->bind();
        _shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
        _shader->setMat4("u_Transform", _transform);

        _vertexArray->bind();
        RenderCommand::drawIndexed(_vertexArray);
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
    void Renderer::draw(const GameObjectPtr& _gameObject) {
        Render2D::draw(_gameObject);
    }

}