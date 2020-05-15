#include "TestGame.h"

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(true), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }),
                        app(Application::get()){
}

void TestGame::onInit() {
    this->proceduralTexture = Texture2D::create(this->app.getWindowSize().x, this->app.getWindowSize().y, true);
    LOG_INFO("w: {0}, h: {1}", this->app.getWindowSize().x, this->app.getWindowSize().y);
    this->initSimulationWorld();
}

void TestGame::onEnd() {
    delete this->particles;
}

void TestGame::onUpdate(engine::Timestep _dt) {
    auto _mousePos = Input::getMousePosition();
    if(_mousePos >= 0 && _mousePos.x < this->proceduralTexture->getWidth() && _mousePos.y < this->proceduralTexture->getHeight()) {
        if (Input::isMousePressed(MouseCode::Button0)) {
            int _posInVector = (int)_mousePos.x + ((int)this->proceduralTexture->getWidth() * (int)_mousePos.y);
            Particle _particle;
            _particle.posInVector   = _posInVector;
            _particle.position      = {_mousePos.x, _mousePos.y};
            _particle.color         = {194, 178, 128, 255};
            _particle.type          = ParticleType::SAND;
            this->particles[_posInVector] = _particle;
        } else if (Input::isMousePressed(MouseCode::Button1)) {
            int _posInVector = (int)_mousePos.x + ((int)this->proceduralTexture->getWidth() * (int)_mousePos.y);
            Particle _particle;
            _particle.posInVector   = _posInVector;
            _particle.position      = {_mousePos.x, _mousePos.y};
            _particle.color         = {90,188,216, 255};
            _particle.type          = ParticleType::WATER;
            this->particles[_posInVector] = _particle;
        }
    }

    for(int _y = 0; _y < this->proceduralTexture->getHeight(); _y++)
        for(int _x = 0; _x < this->proceduralTexture->getWidth(); _x++) {
            int _pos = (int)_x + ((int)this->proceduralTexture->getWidth() * (int)_y);
            this->updateSandParticle(_pos);
            this->updateWaterParticle(_pos);
        }

    for(int _y = 0; _y < this->proceduralTexture->getHeight(); _y++)
        for(int _x = 0; _x < this->proceduralTexture->getWidth(); _x++) {
            int _pos = (int)_x + ((int)this->proceduralTexture->getWidth() * (int)_y);
            this->proceduralTexture->setPixel((int)this->particles[_pos].position.x, (int)this->particles[_pos].position.y, this->particles[_pos].color);
            this->particles[_pos].updated = false;
        }

    this->proceduralTexture->updateTexture();
}

void TestGame::onFixedUpdate(engine::Timestep _dt) {
//    this->world.fixedUpdate(_dt);
}

void TestGame::onRender(engine::Timestep _dt) {
    engine::Render2D::resetStats();
    engine::RenderCommand::setClearColor(engine::Color::Black);
    engine::RenderCommand::clear();

    engine::Render2D::beginRender(this->cameraController.getCamera());
        engine::Render2D::drawTextureRect({0.0f, 0.0f},
                {(float)this->proceduralTexture->getWidth(), (float)this->proceduralTexture->getHeight()}, this->proceduralTexture);
    engine::Render2D::endRender();
}

void TestGame::onImGuiRender(engine::Timestep _dt) {
    engine::ImGuiLayer::drawDebugInfo();
}

void TestGame::onEvent(engine::Event& _e) {
    this->cameraController.onEvent(_e);
}

void TestGame::initSimulationWorld() {
    this->particles = new Particle[this->proceduralTexture->getWidth() * this->proceduralTexture->getHeight()];

        for(int _y = 0; _y < this->app.getWindowSize().y; _y++)
            for(int _x = 0; _x < this->app.getWindowSize().x; _x++) {
                int _posInVector = (int)_x + ((int)this->proceduralTexture->getWidth() * (int)_y);
                Particle _particle;
                _particle.posInVector   = _posInVector;
                _particle.position      = {(float)_x, (float)_y};
                _particle.color         = {0, 0, 0, 0};
                _particle.type          = ParticleType::NONE_PARTICLE;
                this->particles[_posInVector] = _particle;
                this->proceduralTexture->setPixel(_x, _y, _particle.color);

        }

    this->proceduralTexture->updateTexture();

}

void TestGame::updateSandParticle(int _pos) {
    if(this->particles[_pos].type == SAND) {
        if (this->downNeighbour(_pos) && !this->particles[_pos].updated) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = {0, 0, 0, 0};
            this->particles[_pos - this->proceduralTexture->getWidth()].type = SAND;
            this->particles[_pos - this->proceduralTexture->getWidth()].color = {194, 178, 128, 255};
        } else if (this->downLeftNeighbour(_pos)) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = {0, 0, 0, 0};
            this->particles[_pos - this->proceduralTexture->getWidth() - 1].type = SAND;
            this->particles[_pos - this->proceduralTexture->getWidth() - 1].color = {194, 178, 128, 255};
        } else if (this->downRightNeighbour(_pos) && !this->particles[_pos].updated) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = {0, 0, 0, 0};
            this->particles[_pos - this->proceduralTexture->getWidth() + 1].type = SAND;
            this->particles[_pos - this->proceduralTexture->getWidth() + 1].color = {194, 178, 128, 255};
            this->particles[_pos - this->proceduralTexture->getWidth() + 1].updated = true;
        }


    }
}

void TestGame::updateWaterParticle(int _pos) {
    if(this->particles[_pos].type == WATER) {
        if (this->downNeighbour(_pos)) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = {0, 0, 0, 0};
            this->particles[_pos - this->proceduralTexture->getWidth()].type = WATER;
            this->particles[_pos - this->proceduralTexture->getWidth()].color = {90,188,216, 255};
        } else {

            if(this->rightNeighbour(_pos)) {
                this->particles[_pos].type = NONE_PARTICLE;
                this->particles[_pos].color = {0, 0, 0, 0};
                this->particles[_pos + 1].type = WATER;
                this->particles[_pos + 1].color = {90,188,216, 255};
//                this->particles[_pos + 1].updated = true;
            } else if(this->leftNeighbour(_pos)) {
                this->particles[_pos].type = NONE_PARTICLE;
                this->particles[_pos].color = {0, 0, 0, 0};
                this->particles[_pos - 1].type = WATER;
                this->particles[_pos - 1].color = {90,188,216, 255};
            }

        }
    }
}

bool TestGame::downNeighbour(int _pos) {
    return this->particles[_pos].position.y > 0 && this->particles[_pos - this->proceduralTexture->getWidth()].type == NONE_PARTICLE;
}

bool TestGame::leftNeighbour(int _pos) {
    return this->particles[_pos].position.x > 0 && this->particles[_pos - 1].type == ParticleType::NONE_PARTICLE;
}

bool TestGame::rightNeighbour(int _pos) {
    return this->particles[_pos].position.x < (float)this->proceduralTexture->getWidth() - 1 && this->particles[_pos + 1].type == ParticleType::NONE_PARTICLE;
}

bool TestGame::upNeighbour(int _pos) {
    return this->particles[_pos].position.y < (float)this->proceduralTexture->getHeight() - 1 && this->particles[_pos + this->proceduralTexture->getWidth()].type == NONE_PARTICLE;
}

bool TestGame::downLeftNeighbour(int _pos) {
    return this->particles[_pos].position.y > 0 && this->particles[_pos].position.x != 0 &&
            this->particles[_pos - this->proceduralTexture->getWidth() - 1].type == NONE_PARTICLE;
}

bool TestGame::downRightNeighbour(int _pos) {
    return this->particles[_pos].position.x < (float)this->proceduralTexture->getWidth() && this->particles[_pos].position.y != 0 &&
            this->particles[_pos - this->proceduralTexture->getWidth() + 1].type == NONE_PARTICLE;
}
