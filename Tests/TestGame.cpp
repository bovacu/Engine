#include "TestGame.h"
#include <imgui.h>

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(1280.0f / 720.0f), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }) {
}

void TestGame::onInit() {
    this->world.init();

    this->checkerboardTexture = engine::Texture2D::create("assets/textures/Checkerboard2.png");
    this->emeraldTextures = engine::Texture2D::create("assets/textures/emerald.png");
    this->playerTexture = engine::Texture2D::create("assets/textures/trainer.png");

    this->player = engine::GameObject::create({0.0f, 0.0f});
    auto _body = this->player->addPhysicsBody();
    _body->bodyType = engine::BodyType::DYNAMIC;
    _body->movementType = engine::MovementType::LINEAR;
    auto _sprite = this->player->addSprite(engine::TextureRegion::create(this->playerTexture, {0, 2}, {16.0f, 21.0f}));
    this->player->addBoxCollider(_sprite->getSize());

    this->rock = engine::GameObject::create({2.0f, 0.f});
    auto _sprite2 = this->rock->addSprite(engine::TextureRegion::create(this->emeraldTextures, {4, 18}, {16.f, 16.f}));
    this->rock->addPhysicsBody();
    this->rock->addBoxCollider(_sprite2->getSize());

    this->world.addGameObject(this->player);
    this->world.addGameObject(this->rock);
}

void TestGame::onEnd() {  }

void TestGame::onUpdate(engine::Timestep _dt) {

    auto _body = this->player->getComponentOfType<engine::PhysicsBody>();

    if(engine::Input::isKeyPressed(engine::KeyCode::D))
        _body->velocity.x = 2.5;
    else if(engine::Input::isKeyPressed(engine::KeyCode::A))
        _body->velocity.x = -2.5;
    else
        _body->velocity.x = 0;

    if(engine::Input::isKeyPressed(engine::KeyCode::W))
        _body->velocity.y = 2.5;
    else if(engine::Input::isKeyPressed(engine::KeyCode::S))
        _body->velocity.y = -2.5;
    else
        _body->velocity.y = 0;

    this->world.update(_dt);

    this->player->getComponentOfType<engine::Sprite>()->setPosition(this->player->transform.position);
    this->cameraController.setCameraPosition(this->player->transform.position);
    this->cameraController.onUpdate(_dt);
}

void TestGame::onFixedUpdate(engine::Timestep _dt) {
    this->world.fixedUpdate(_dt);
}

void TestGame::onRender(engine::Timestep _dt) {
    engine::Render2D::resetStats();
    engine::RenderCommand::setClearColor(engine::Color::Black);
    engine::RenderCommand::clear();

    static float rotation = 0.0f;
    rotation += _dt * 50.0f;
    std::vector<engine::Vec2f> _vertices = { {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {0.0f, 0.6f}, {-0.5f, 0.5f} };
    engine::PolygonShape _triangle(_vertices, {-1, 1});

    engine::Render2D::beginRender(this->cameraController.getCamera());
        engine::Render2D::drawTextureRect({0.0f, 0.0f}, {20.0f, 20.0f}, this->checkerboardTexture);
        engine::Render2D::draw(this->rock);
        engine::Render2D::draw(this->player);
        engine::Render2D::drawRect({ASPECT_RATIO_PIXEL * 32.5f, ASPECT_RATIO_PIXEL / 2.f}, {ASPECT_RATIO_PIXEL, ASPECT_RATIO_PIXEL}, engine::Color::Red);
        engine::Render2D::draw(_triangle);
    engine::Render2D::endRender();

    this->world.debugWorld(this->cameraController.getCamera());
}

void TestGame::onImGuiRender(engine::Timestep _dt) {
    engine::ImGuiLayer::drawDebugInfo();
}

void TestGame::onEvent(engine::Event& _e) {
    this->cameraController.onEvent(_e);
}