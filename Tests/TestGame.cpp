#include "TestGame.h"
#include <imgui.h>

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(true), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }),
                        app(Application::get()) {
}

void TestGame::onInit() {
    this->app.setTitle("Particle simulator");
    this->proceduralTexture = Texture2D::create((uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y, true);

    this->initSimulationWorld();

    this->pauseTexture = engine::ImGuiTexture2D::create("assets/textures/pause.png");
    this->resumeTexture = engine::ImGuiTexture2D::create("assets/textures/play-button.png");
    this->advanceTexture = engine::ImGuiTexture2D::create("assets/textures/fast-forward.png");
    this->oneFrameTexture = engine::ImGuiTexture2D::create("assets/textures/archivo-de-video.png");
    this->drawTexture = engine::ImGuiTexture2D::create("assets/textures/editar.png");
    this->eraseTexture = engine::ImGuiTexture2D::create("assets/textures/borrador.png");

    this->enabledTexture = engine::ImGuiTexture2D::create("assets/textures/selected.png");
    this->disabledTexture = engine::ImGuiTexture2D::create("assets/textures/unselected.png");
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
        if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemActive() && !this->tools[IMGUI_CRAFTER]) {
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
            if(this->random.random<int>(0, 1) == 0) {
                for (int _x = 0; _x < (int) this->proceduralTexture->getWidth(); _x++) {
                    int _pos = this->calcVecPos(_x, _y);
                    ParticleType _type = this->particles[_pos].type;
                    switch (_type) {
                        case SAND   : this->updateSandParticle(_x, _y, _pos, _dt);  break;
                        case WATER  : this->updateWaterParticle(_x, _y, _pos, _dt); break;
                        case ROCK   : this->updateRockParticle(_x, _y, _pos, _dt);  break;
                        default     :                                               break;
                    }
                }
            } else {
                for (int _x = (int)this->proceduralTexture->getWidth() - 1; _x > 0; _x--) {
                    int _pos = this->calcVecPos(_x, _y);
                    ParticleType _type = this->particles[_pos].type;
                    switch (_type) {
                        case SAND   : this->updateSandParticle(_x, _y, _pos, _dt);  break;
                        case WATER  : this->updateWaterParticle(_x, _y, _pos, _dt); break;
                        case ROCK   : this->updateRockParticle(_x, _y, _pos, _dt);  break;
                        default     :                                               break;
                    }
                }
            }
        }

        /// UPDATING PIXELS
//        for(int _y = 0; _y < (int)this->proceduralTexture->getHeight(); _y++) {
//            for (int _x = 0; _x < (int) this->proceduralTexture->getWidth(); _x++) {
//                int _pos = this->calcVecPos(_x, _y);
//                this->particles[_pos].updated = false;
//            }
//        }

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
    this->imGuiToolEnablerWindow(_dt);

//    if(this->tools[IMGUI_DEBUGGING])
//        engine::ImGuiLayer::drawDebugInfo();
//
//    if(this->tools[IMGUI_DRAWING])
//        this->imGuiDrawingWindow(_dt);
//
//    if(this->tools[IMGUI_CONTROLLER])
//        this->imGuiControllerWindow(_dt);
//
//    if(this->tools[IMGUI_CRAFTER])
//        this->imGuiCrafterWindow(_dt);

    this->particlesUpdating = 0;
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
                _particle.velocity      = {0, 0};
                _particle.color         = {0, 0, 0, 0};
                _particle.type          = ParticleType::NONE_PARTICLE;
                this->particles[_posInVector] = _particle;
                this->proceduralTexture->setPixel(_x, _y, _particle.color);

        }

    this->proceduralTexture->updateTexture();

}

void TestGame::updateSandParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->gravity * _dt), -this->gravity, this->gravity );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= 0.5f;

    Particle _tempA = *_p;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        _tempB = this->particles[this->calcVecPos(_vX, _vY)];
        _tempA.velocity.x = 0;
        this->writeParticle(_vX, _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);
        this->particlesUpdating++;
    } else {
        if(this->isEmpty(_x, _y - 1)) {
            _p->velocity.y += (this->gravity * _dt);
            _tempB = this->particles[this->calcVecPos(_x, _y - 1)];

            this->writeParticle(_x, _y - 1, *_p);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        } else if (this->isEmpty(_x - 1, _y - 1)) {
            _p->velocity.y += (this->gravity * _dt);
                _p->velocity.x = this->random.random<int>( 0, 1 ) == 0 ? -1.f : 1.f;
//                _p->velocity.x = -2.f;
//            _p->velocity.x = -1.f;

            _tempB = this->particles[this->calcVecPos(_x - 1, _y - 1)];
            this->writeParticle(_x - 1, _y - 1, *_p);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        } else if (this->isEmpty(_x + 1, _y - 1)) {
            _p->velocity.y += (this->gravity * _dt);
                _p->velocity.x = this->random.random<int>( 0, 1 ) == 0 ? -1.f : 1.f;
//            _p->velocity.x = 1.f;

            _tempB = this->particles[this->calcVecPos(_x + 1, _y - 1)];
            this->writeParticle(_x + 1, _y - 1, *_p);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }
    }

    _p->updated = true;
}

void TestGame::updateWaterParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    int _sign = this->random.random<int>(0, 1) == 0 ? -1 : 1;
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->gravity * _dt), -this->gravity, this->gravity );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= 0.5f;

    Particle _tempA = *_p;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        _tempB = this->particles[this->calcVecPos(_vX, _vY)];
        this->writeParticle(_vX, _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);
        this->particlesUpdating++;
    } else {

        int _below = this->calcVecPos(_x, _y - 1);

        int _firstMovement  = this->calcVecPos(_x + _sign, _y);
        int _secondMovement = this->calcVecPos(_x - _sign, _y);

        int _firstDownMovement  = this->calcVecPos(_x + _sign, _y - 1);
        int _secondDownMovement = this->calcVecPos(_x - _sign, _y - 1);

        /// Down
        if(this->isInBounds(_x, _y - 1) && (_tempB = this->particles[_below]).type == NONE_PARTICLE) {
            this->writeParticle(_x, _y - 1, _tempA);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }

        /// Down-Right OR Down-Left
        else if (this->isInBounds(_x + _sign, _y - 1) && (_tempB = this->particles[_firstDownMovement]).type == NONE_PARTICLE) {
            this->writeParticle(_x + _sign, _y - 1, _tempA);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }

        /// Down-Right OR Down-Left
        else if (this->isInBounds(_x - _sign, _y - 1) && (_tempB = this->particles[_secondDownMovement]).type == NONE_PARTICLE) {
            this->writeParticle(_x - _sign, _y - 1, _tempA);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }

        /// Left OR Right
        else if (this->isInBounds(_x + _sign, _y) && (_tempB = this->particles[_firstMovement]).type == NONE_PARTICLE) {
            this->writeParticle(_x + _sign, _y, _tempA);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }

        /// Left OR Right
        else if (this->isInBounds(_x - _sign, _y) && (_tempB = this->particles[_secondMovement]).type == NONE_PARTICLE) {
            this->writeParticle(_x - _sign, _y, _tempA);
            this->writeParticle(_x, _y, _tempB);
            this->particlesUpdating++;
        }
    }
}

void TestGame::updateRockParticle(int _x, int _y, int _posInVector, Timestep _dt) {

}

Color TestGame::particleTypeToColor(const TestGame::ParticleType& _particle) {
    switch (_particle) {
        case SAND           : return this->PARTICLE_COLORS[this->random.random(0, 4)];
        case WATER          : return this->PARTICLE_COLORS[5];
        case ROCK           : return this->PARTICLE_COLORS[6];
        case NONE_PARTICLE  : return this->PARTICLE_COLORS[7];
    }

    return this->PARTICLE_COLORS[7];
}

void TestGame::generateParticles(const Vec2f& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    if(this->particles[_posInVector].type == NONE_PARTICLE) {
        Particle _particle;
        _particle.velocity              = {0, 0};
        _particle.color                 = this->particleTypeToColor(this->selectedParticle);
        _particle.type                  = this->selectedParticle;
        this->particles[_posInVector]   = _particle;

        this->writeParticle((int)_mousePos.x, (int)_mousePos.y, this->particles[_posInVector]);
    }
}

void TestGame::generateWithBrush(const Vec2f& _mousePos) {

    if(this->brushSize == 1) {
        this->generateParticles(_mousePos);
    } else {
        if(this->selectedParticle != ROCK) {
            std::vector<std::pair<int, int>> _spawnedPositions;
            for(int _i = 0; _i < this->brushSize; _i++) {
                auto _randPos = std::make_pair<int, int>(this->random.random<int>(-this->brushSize, this->brushSize), this->random.random<int>(-this->brushSize, this->brushSize));
                while(std::find(_spawnedPositions.begin(), _spawnedPositions.end(), _randPos) != _spawnedPositions.end() ||
                      (_randPos.first  + _mousePos.x < 0 || _randPos.first  + _mousePos.x >= (float)this->proceduralTexture->getWidth() - 1) ||
                      (_randPos.second + _mousePos.y < 0 || _randPos.second + _mousePos.y >= (float)this->proceduralTexture->getHeight() - 1))
                    _randPos = std::make_pair<int, int>(this->random.random<int>(-this->brushSize, this->brushSize), this->random.random<int>(-this->brushSize, this->brushSize));

                _spawnedPositions.emplace_back(_randPos);
                this->generateParticles({_mousePos.x + _randPos.first, _mousePos.y + _randPos.second});
            }
        } else {
            std::vector<std::pair<int, int>> _spawnedPositions;
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
    return _x >= 0 && _x <= (int)this->proceduralTexture->getWidth() - 1 &&
           _y >= 0 && _y <= (int)this->proceduralTexture->getHeight() - 1;
}

int TestGame::calcVecPos(int _x, int _y) {
    return _x + ((int)this->proceduralTexture->getWidth() * _y);
}

bool TestGame::isEmpty(int _x, int _y) {
    return this->isInBounds(_x, _y) && this->particles[this->calcVecPos(_x, _y)].type == NONE_PARTICLE;
}

void TestGame::writeParticle(int _x, int _y, const TestGame::Particle& _particle) {
    this->particles[this->calcVecPos(_x, _y)] = _particle;
    this->proceduralTexture->setPixel(_x, _y, _particle.color);
}

void TestGame::imGuiToolEnablerWindow(engine::Timestep _dt) {
    ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
//        ImGui::Text("Debugging"); ImGui::SameLine(80);
//        if(this->tools[IMGUI_DEBUGGING]) {
//            if(ImGui::ImageButton((void*)(intptr_t)this->enabledTexture->getTexture(), ImVec2((float)this->enabledTexture->getWidth(), (float)this->enabledTexture->getHeight())))
//                this->tools[IMGUI_DEBUGGING] = false;
//        } else {
//            if(ImGui::ImageButton((void*)(intptr_t)this->disabledTexture->getTexture(), ImVec2((float)this->disabledTexture->getWidth(), (float)this->disabledTexture->getHeight())))
//                this->tools[IMGUI_DEBUGGING] = true;
//        }
//
//        ImGui::Text("Controller"); ImGui::SameLine(80);
//        if(this->tools[IMGUI_CONTROLLER]) {
//            if(ImGui::ImageButton((void*)(intptr_t)this->enabledTexture->getTexture(), ImVec2((float)this->enabledTexture->getWidth(), (float)this->enabledTexture->getHeight())))
//                this->tools[IMGUI_CONTROLLER] = false;
//        } else {
//            if(ImGui::ImageButton((void*)(intptr_t)this->disabledTexture->getTexture(), ImVec2((float)this->disabledTexture->getWidth(), (float)this->disabledTexture->getHeight())))
//                this->tools[IMGUI_CONTROLLER] = true;
//        }
//
//        ImGui::Text("Drawing"); ImGui::SameLine(80);
//        if(this->tools[IMGUI_DRAWING]) {
//            if(ImGui::ImageButton((void*)(intptr_t)this->enabledTexture->getTexture(), ImVec2((float)this->enabledTexture->getWidth(), (float)this->enabledTexture->getHeight())))
//                this->tools[IMGUI_DRAWING] = false;
//        } else {
//            if(ImGui::ImageButton((void*)(intptr_t)this->disabledTexture->getTexture(), ImVec2((float)this->disabledTexture->getWidth(), (float)this->disabledTexture->getHeight())))
//                this->tools[IMGUI_DRAWING] = true;
//        }

        ImGui::Text("Crafter"); ImGui::SameLine(80);
        if(ImGui::Button("Open Crafter")) {
            ImGui::SetNextWindowSize({(float)this->app.getWindowSize().x / 2.f, (float)this->app.getWindowSize().y / 2.f}, ImGuiCond_Always);
            ImGui::SetNextWindowPos({(float)this->app.getWindowSize().x, (float)this->app.getWindowSize().y}, ImGuiCond_Always);
            ImGui::OpenPopup("Crafter");
            this->tools[IMGUI_CRAFTER] = true;
            this->play = false;
        }

        this->imGuiCrafterWindow(_dt);

    ImGui::End();
}

void TestGame::imGuiDrawingWindow(engine::Timestep _dt) {
    ImGui::Begin("Drawing");
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

void TestGame::imGuiControllerWindow(engine::Timestep _dt) {
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
    ImGui::End();
}

void TestGame::imGuiCrafterWindow(engine::Timestep _dt) {


    if(ImGui::BeginPopupModal("Crafter", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("cosita");
        if(ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            this->tools[IMGUI_CRAFTER] = false;
            this->play = true;
        }

        ImGui::EndPopup();
    }
}
