#include "TestGame.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(1280.0f / 720.0f), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }) {  }

void TestGame::onInit() {
    this->checkerboardTexture = engine::Texture2D::create("assets/textures/Checkerboard.png");
}

void TestGame::onEnd() {  }

void TestGame::onUpdate(engine::Timestep _ts) {
    // Update
    this->cameraController.onUpdate(_ts);
}

void TestGame::onFixedUpdate(engine::Timestep _dt) {

}

void TestGame::onRender(engine::Timestep _dt) {
    engine::Render2D::resetStats();
    engine::RenderCommand::setClearColor(engine::Color::Black);
    engine::RenderCommand::clear();

    static float rotation = 0.0f;
    rotation += _dt * 50.0f;

    engine::Render2D::beginRender(this->cameraController.getCamera());
        engine::Render2D::drawRotatedRect({1.0f, 0.0f}, {0.8f, 0.8f}, -45.0f, engine::Color::Red);
        engine::Render2D::drawRect({-1.0f, 0.0f}, {0.8f, 0.8f}, engine::Color::Green);
        engine::Render2D::drawRect({0.5f, -0.5f}, {0.5f, 0.75f}, engine::Color::Magenta);
        engine::Render2D::drawRect({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, this->checkerboardTexture, 10.0f);
        engine::Render2D::drawRotatedRect({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, rotation, this->checkerboardTexture, 20.0f);
    engine::Render2D::endRender();
}

void TestGame::onImGuiRender(engine::Timestep _dt) {
    engine::ImGuiLayer::drawDebugInfo();
}

void TestGame::onEvent(engine::Event& e) {
    this->cameraController.onEvent(e);
}