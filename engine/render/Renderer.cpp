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

    void Renderer::beginRender(OrthographicCamera& _camera) {
        sceneData->viewProjectionMatrix = _camera.getViewProjectionMatrix();
    }

    void Renderer::endRender() {
    }

    void Renderer::submit(const ShaderPtr& _shader, const VertexArrayPtr& _vertexArray, const glm::mat4& _transform) {
        _shader->bind();
        _shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
        _shader->setMat4("u_Transform", _transform);

        _vertexArray->bind();
        RenderCommand::drawIndexed(_vertexArray);
    }

}