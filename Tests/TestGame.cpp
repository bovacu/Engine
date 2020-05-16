#include "TestGame.h"
#include <imgui.h>

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(true), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }),
                        app(Application::get()), mt(rd()) {
}

void TestGame::onInit() {
    this->app.setTitle("Particle simulator");

    this->proceduralTexture = Texture2D::create((uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y, true);
//    _t->LoadTextureFromFile("assets/textures/Checkerboard.png", &this->testTexture, &this->testWidth, &this->testHeight);
    LOG_INFO("w: {0}, h: {1}", this->app.getWindowSize().x, this->app.getWindowSize().y);
    this->initSimulationWorld();
    this->pauseTexture = engine::ImGuiTexture2D::create("assets/textures/pause.png");
    this->resumeTexture = engine::ImGuiTexture2D::create("assets/textures/play-button.png");
    this->advanceTexture = engine::ImGuiTexture2D::create("assets/textures/fast-forward.png");
    this->oneFrameTexture = engine::ImGuiTexture2D::create("assets/textures/archivo-de-video.png");
    this->drawTexture = engine::ImGuiTexture2D::create("assets/textures/editar.png");
    this->eraseTexture = engine::ImGuiTexture2D::create("assets/textures/borrador.png");
}

void TestGame::onEnd() {
    delete this->particles;
}

void TestGame::onUpdate(engine::Timestep _dt) {
    if(this->play) {
        if(this->oneStep) {
            this->play = false;
            this->oneStep = false;
        }

        /// ADDING NEW PARTICLES
        if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemActive()) {
            auto _mousePos = Input::getMousePosition();
            if(_mousePos >= 0 && _mousePos.x < this->proceduralTexture->getWidth() && _mousePos.y < this->proceduralTexture->getHeight()) {
                if (Input::isMousePressed(MouseCode::Button0)) {
                    if(this->usingTool == DRAW)
                        this->generateWithBrush(_mousePos);
                    else
                        this->removeParticles(_mousePos);
                }
            }
        }

        /// UPDATING PARTICLES
        for(int _y = 0; _y < (int)this->proceduralTexture->getHeight(); _y++) {
            for (int _x = 0; _x < (int) this->proceduralTexture->getWidth(); _x++) {
                this->updateSandParticle(_x, _y);
//                this->updateWaterParticle(_pos);
            }
        }

        /// UPDATING PIXELS
        for(int _y = 0; _y < (int)this->proceduralTexture->getHeight(); _y++) {
            for (int _x = 0; _x < (int) this->proceduralTexture->getWidth(); _x++) {
                int _pos = this->calcVecPos(_x, _y);
                this->proceduralTexture->setPixel((int) this->particles[_pos].position.x,
                                                  (int) this->particles[_pos].position.y, this->particles[_pos].color);
                this->particles[_pos].updated = false;
            }
        }

        this->proceduralTexture->updateTexture();
    }
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

    ImGui::Begin("Controller");

        if(ImGui::Button("Reset Scene")) {
            delete [] this->particles;
            this->initSimulationWorld();
        }

        ImGui::Separator();

        if(ImGui::ImageButton((void*)(intptr_t)this->resumeTexture->getTexture(), ImVec2((float)this->resumeTexture->getWidth(), (float)this->resumeTexture->getHeight())))
            this->play = true;
        ImGui::SameLine();
        if(ImGui::ImageButton((void*)(intptr_t)this->pauseTexture->getTexture(), ImVec2((float)this->pauseTexture->getWidth(), (float)this->pauseTexture->getHeight())))
            this->play = false;
        ImGui::SameLine();
        if(ImGui::ImageButton((void*)(intptr_t)this->oneFrameTexture->getTexture(), ImVec2((float)this->oneFrameTexture->getWidth(), (float)this->oneFrameTexture->getHeight()))) {
            this->play = true;
            this->oneStep = true;
        }
        ImGui::SameLine();
        ImGui::ImageButton((void*)(intptr_t)this->advanceTexture->getTexture(), ImVec2((float)this->advanceTexture->getWidth(), (float)this->advanceTexture->getHeight()));
        if(ImGui::IsItemHovered() && Input::isMousePressed(MouseCode::Button0)) {
            this->play = true;
            this->oneStep = true;
        }

        ImGui::Separator();

        if(ImGui::ImageButton((void*)(intptr_t)this->drawTexture->getTexture(), ImVec2((float)this->drawTexture->getWidth(), (float)this->drawTexture->getHeight()))) {
            this->usingTool = DRAW;
        }
        ImGui::SameLine();
        if(ImGui::ImageButton((void*)(intptr_t)this->eraseTexture->getTexture(), ImVec2((float)this->eraseTexture->getWidth(), (float)this->eraseTexture->getHeight()))) {
            this->usingTool = ERASE;
        }

        const char* _materials[] = { "Sand", "Water", "Rock"};
        static const char* _materialSelected = _materials[0];

        ImGui::Separator();
        ImGui::Text("Materials");
        if (ImGui::BeginCombo("##combo", _materialSelected)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _material : _materials) {
                bool is_selected = (_materialSelected == _material); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_material, is_selected)) {
                    _materialSelected = _material;
                    if(strcmp(_materialSelected, "Sand") == 0)
                        this->selectedParticle = SAND;
                    else if(strcmp(_materialSelected, "Water") == 0)
                        this->selectedParticle = WATER;
                    else if(strcmp(_materialSelected, "Rock") == 0)
                        this->selectedParticle = ROCK;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        ImGui::Text("Brush thickness");
        ImGui::SliderInt("##slider", &this->brushSize, 1, MAX_BRUSH_THICKNESS);

    ImGui::End();
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
                _particle.position      = {(float)_x, (float)_y};
                _particle.color         = {0, 0, 0, 0};
                _particle.type          = ParticleType::NONE_PARTICLE;
                this->particles[_posInVector] = _particle;
                this->proceduralTexture->setPixel(_x, _y, _particle.color);

        }

    this->proceduralTexture->updateTexture();

}

void TestGame::updateSandParticle(int _x, int _y) {
    int _pos = this->calcVecPos(_x, _y);
    if(this->particles[_pos].type == SAND) {

        int _neighbours = 0;
        while(this->isInBounds(_x, _y - 1 - _neighbours) && this->isEmpty(_x, _y - 1 - _neighbours) &&
                _neighbours <= SAND_MAX_FALL) { _neighbours++; }

        if (_neighbours >= 1) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = TRANSPARENT_COLOR;

            int _toExchange = this->calcVecPos(_x, _y - _neighbours);
            this->particles[_toExchange].type = SAND;
            this->particles[_toExchange].color = SAND_COLOR;
            return;
        }

        while(this->isInBounds( _x - 1 - _neighbours, _y - 1 - _neighbours) && this->isEmpty(_x - 1 - _neighbours, _y - 1 - _neighbours) &&
              _neighbours <= SAND_MAX_FALL) { _neighbours++; }

        if (_neighbours >= 1) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = TRANSPARENT_COLOR;

            int _toExchange = this->calcVecPos(_x - _neighbours, _y - _neighbours);
            this->particles[_toExchange].type = SAND;
            this->particles[_toExchange].color = SAND_COLOR;
            return;
        }

        while(this->isInBounds( _x + 1 + _neighbours, _y - 1 - _neighbours) && this->isEmpty(_x + 1 + _neighbours, _y - 1 - _neighbours) &&
              _neighbours <= SAND_MAX_FALL) { _neighbours++; }

        if (_neighbours >= 1) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = TRANSPARENT_COLOR;

            int _toExchange = this->calcVecPos(_x + _neighbours, _y - _neighbours);
            this->particles[_toExchange].type = SAND;
            this->particles[_toExchange].color = SAND_COLOR;
            this->particles[_toExchange].updated = true;
            return;
        }
    }
}

void TestGame::updateWaterParticle(int _pos) {
    if(this->particles[_pos].type == WATER) {
        if (this->downNeighbour(_pos)) {
            this->particles[_pos].type = NONE_PARTICLE;
            this->particles[_pos].color = TRANSPARENT_COLOR;
            this->particles[_pos - this->proceduralTexture->getWidth()].type = WATER;
            this->particles[_pos - this->proceduralTexture->getWidth()].color = WATER_COLOR;
        } else {

            if(this->rightNeighbour(_pos)) {
                this->particles[_pos].type = NONE_PARTICLE;
                this->particles[_pos].color = TRANSPARENT_COLOR;
                this->particles[_pos + 1].type = WATER;
                this->particles[_pos + 1].color = WATER_COLOR;
//                this->particles[_pos + 1].updated = true;
            } else if(this->leftNeighbour(_pos)) {
                this->particles[_pos].type = NONE_PARTICLE;
                this->particles[_pos].color = TRANSPARENT_COLOR;
                this->particles[_pos - 1].type = WATER;
                this->particles[_pos - 1].color = WATER_COLOR;
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
    return this->particles[_pos].position.x < (float)this->proceduralTexture->getWidth() - 1 && this->particles[_pos].position.y != 0 &&
            this->particles[_pos - this->proceduralTexture->getWidth() + 1].type == NONE_PARTICLE;
}

Color TestGame::particleTypeToColor(const TestGame::ParticleType& _particle) {
    switch (_particle) {
        case SAND           : return SAND_COLOR;
        case WATER          : return WATER_COLOR;
        case ROCK           : return ROCK_COLOR;
        case NONE_PARTICLE  : return TRANSPARENT_COLOR;
    }

    return TRANSPARENT_COLOR;
}

void TestGame::generateParticles(const Vec2f& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    if(this->particles[_posInVector].type == NONE_PARTICLE) {
        Particle _particle;
        _particle.position              = {_mousePos.x, _mousePos.y};
        _particle.color                 = this->particleTypeToColor(this->selectedParticle);
        _particle.type                  = this->selectedParticle;
        this->particles[_posInVector]   = _particle;
    }
}

void TestGame::generateWithBrush(const Vec2f& _mousePos) {

    if(this->brushSize == 1) {
        this->generateParticles(_mousePos);
    } else {
        if(this->selectedParticle != ROCK) {
            std::vector<std::pair<int, int>> _spawnedPositions;
            std::uniform_int_distribution<int> dist(-this->brushSize, this->brushSize);
            for(int _i = 0; _i < this->brushSize; _i++) {
                auto _randPos = std::make_pair<int, int>(dist(mt), dist(mt));
                while(std::find(_spawnedPositions.begin(), _spawnedPositions.end(), _randPos) != _spawnedPositions.end() ||
                      (_randPos.first  + _mousePos.x < 0 || _randPos.first  + _mousePos.x >= (float)this->proceduralTexture->getWidth() - 1) ||
                      (_randPos.second + _mousePos.y < 0 || _randPos.second + _mousePos.y >= (float)this->proceduralTexture->getHeight() - 1))
                    _randPos = std::make_pair<int, int>(dist(mt), dist(mt));

                _spawnedPositions.emplace_back(_randPos);
                this->generateParticles({_mousePos.x + _randPos.first, _mousePos.y + _randPos.second});
            }
        } else {
            std::vector<std::pair<int, int>> _spawnedPositions;
            std::uniform_int_distribution<int> dist(-this->brushSize, this->brushSize);
            for(int _y = -this->brushSize / 2; _y < this->brushSize / 2; _y++) {
                for(int _x = -this->brushSize / 2; _x < this->brushSize / 2; _x++) {
                    if(((float)_x  + _mousePos.x > 0 || (float)_x  + _mousePos.x <= (float)this->proceduralTexture->getWidth() - 1) ||
                       ((float)_y + _mousePos.y > 0 || (float)_y + _mousePos.y <= (float)this->proceduralTexture->getHeight() - 1)) {
                        this->generateParticles({_mousePos.x + (float)_x, _mousePos.y + (float)_y});
                    }
                }
            }
        }
    }
}

void TestGame::removeParticles(const Vec2f& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    this->particles[_posInVector].type  = NONE_PARTICLE;
    this->particles[_posInVector].color = this->particleTypeToColor(NONE_PARTICLE);
}

bool TestGame::isInBounds(int _x, int _y) {
    return _x >= 0 && _x <= this->proceduralTexture->getWidth() - 1 &&
           _y >= 0 && _y <= this->proceduralTexture->getHeight() - 1;
}

int TestGame::calcVecPos(int _x, int _y) {
    return _x + ((int)this->proceduralTexture->getWidth() * _y);
}

bool TestGame::isEmpty(int _x, int _y) {
    return this->particles[this->calcVecPos(_x, _y)].type == NONE_PARTICLE;
}
