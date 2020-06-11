#include "TestGame.h"
#include <imgui.h>

TestGame::Particle TestGame::noneParticle;
TestGame::Particle TestGame::sandParticle;
TestGame::Particle TestGame::waterParticle;
TestGame::Particle TestGame::stoneParticle;
TestGame::Particle TestGame::acidParticle;
TestGame::Particle TestGame::dirtParticle;

int TestGame::textureWidth = 0;
int TestGame::textureHeight = 0;

TestGame::TestGame() :  engine::Layer("Prueba"),
                        cameraController(true), squareColor({ 0.2f, 0.3f, 0.8f, 1.0f }),
                        app(Application::get()) {
}

void TestGame::onInit() {
    this->app.setTitle("Particle simulator");
    this->proceduralTexture = Texture2D::create((uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y, true);

    TestGame::textureWidth  = this->proceduralTexture->getWidth();
    TestGame::textureHeight = this->proceduralTexture->getHeight();
    this->totalOfPixels = TestGame::textureWidth * TestGame::textureHeight;
    this->drawnPixels = 0;

    this->initSimulationWorld();

    this->pauseTexture = engine::ImGuiTexture2D::create("assets/textures/pause.png");
    this->resumeTexture = engine::ImGuiTexture2D::create("assets/textures/play-button.png");
    this->advanceTexture = engine::ImGuiTexture2D::create("assets/textures/fast-forward.png");
    this->oneFrameTexture = engine::ImGuiTexture2D::create("assets/textures/archivo-de-video.png");
    this->drawTexture = engine::ImGuiTexture2D::create("assets/textures/editar.png");
    this->eraseTexture = engine::ImGuiTexture2D::create("assets/textures/borrador.png");
    this->zoomTexture = engine::ImGuiTexture2D::create("assets/textures/buscar.png");
    this->pointTexture = engine::ImGuiTexture2D::create("assets/textures/point.png");
}
void TestGame::onEvent(engine::Event& _e) {
    this->cameraController.onEvent(_e);

    if(_e.getEventType() == EventType::MouseScrolled && this->usingTool == ZOOM) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(TestGame::onMouseScrolled));
    }
}
void TestGame::onUpdate(engine::Timestep _dt) {

    this->checkForShortcuts();

    if(Input::isKeyJustPressed(KEY_SPACE))
        this->play = !this->play;

    if(this->play) {
        if(this->oneStep) {
            this->play = false;
            this->oneStep = false;
        }

        int _textureWidth = this->proceduralTexture->getWidth();

        this->rain();

        /// ADDING NEW PARTICLES
        if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemActive()) {
            auto _mousePos = Input::getMousePosition();
            if(_mousePos >= 0 && _mousePos.x < this->proceduralTexture->getWidth() && _mousePos.y < this->proceduralTexture->getHeight()) {
                if (Input::isMousePressed(MouseCode::Button0)) {
                    if(this->usingTool == DRAW)
                        this->generateWithBrush(_mousePos);
                    else if (this->usingTool == ERASE)
                        this->removeParticles(_mousePos);
                }
            }
        }

        /// UPDATING PARTICLES
        for(int _y = 0; _y < (int)this->proceduralTexture->getHeight(); _y++) {
            if(this->random.randomi(0, 1) == 0) {
                for (int _x = 0; _x < (int) this->proceduralTexture->getWidth(); _x++) {
                    int _pos = _x + _textureWidth * _y;
                    ParticleType _type = this->particles[_pos].type;
                    if(_type == ParticleType::NONE_PARTICLE || _type == ParticleType::STONE || !this->particles[_pos].canUpdate) continue;
                    this->particlesUpdating++;
                    switch (_type) {
                        case SAND   : this->updateSandParticle(_x, _y, _pos, _dt);  break;
                        case WATER  : this->updateWaterParticle(_x, _y, _pos, _dt); break;
                        case ACID   : this->updateAcidParticle(_x, _y, _pos, _dt);  break;
                        case STONE  : this->updateStoneParticle(_x, _y, _pos, _dt); break;
                        case DIRT   : this->updateDirtParticle(_x, _y, _pos, _dt);  break;
                        default     :                                               break;
                    }
                }
            } else {
                for (int _x = (int)this->proceduralTexture->getWidth() - 1; _x > 0; _x--) {
                    int _pos = _x + _textureWidth * _y;
                    ParticleType _type = this->particles[_pos].type;
                    if(_type == ParticleType::NONE_PARTICLE || _type == ParticleType::STONE || !this->particles[_pos].canUpdate) continue;
                    this->particlesUpdating++;
                    switch (_type) {
                        case SAND   : this->updateSandParticle(_x, _y, _pos, _dt);  break;
                        case WATER  : this->updateWaterParticle(_x, _y, _pos, _dt); break;
                        case ACID   : this->updateAcidParticle(_x, _y, _pos, _dt);  break;
                        case STONE  : this->updateStoneParticle(_x, _y, _pos, _dt); break;
                        case DIRT   : this->updateDirtParticle(_x, _y, _pos, _dt);  break;
                        default     :                                               break;
                    }
                }
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
    engine::RenderCommand::setClearColor(this->backgroundColor);
    engine::RenderCommand::clear();

    engine::Render2D::beginRender(this->cameraController.getCamera());
        engine::Render2D::drawTextureRect({0.0f, 0.0f},
                {(float)this->proceduralTexture->getWidth(), (float)this->proceduralTexture->getHeight()}, this->proceduralTexture);
    engine::Render2D::endRender();
}
void TestGame::onImGuiRender(engine::Timestep _dt) {
    this->imGuiAppWindow(_dt);
    this->particlesUpdating = 0;
}
void TestGame::onEnd() {
    delete this->particles;
}

void TestGame::initSimulationWorld() {
    this->particles = new Particle[this->proceduralTexture->getWidth() * this->proceduralTexture->getHeight()];

    for(int _y = 0; _y < this->app.getWindowSize().y; _y++) {
        for (int _x = 0; _x < this->app.getWindowSize().x; _x++) {
            int _posInVector = this->calcVecPos(_x, _y);
            this->particles[_posInVector] = TestGame::noneParticle;
            this->proceduralTexture->setPixel(_x, _y, TestGame::noneParticle.color);
        }
    }

    TestGame::sandParticle.type = SAND;
    TestGame::sandParticle.color = this->particleTypeToColor(SAND);

    TestGame::waterParticle.type = WATER;
    TestGame::waterParticle.color = this->particleTypeToColor(WATER);

    TestGame::stoneParticle.type = STONE;
    TestGame::stoneParticle.color = this->particleTypeToColor(STONE);

    TestGame::acidParticle.type = ACID;
    TestGame::acidParticle.color = this->particleTypeToColor(ACID);

    TestGame::dirtParticle.type = DIRT;
    TestGame::dirtParticle.color = this->particleTypeToColor(DIRT);

    this->proceduralTexture->updateTexture();

}
void TestGame::checkForShortcuts() {
    if(Input::isKeyJustPressed(KEY_1))
        this->usingTool = DRAW;
    else if(Input::isKeyJustPressed(KEY_2))
        this->usingTool = ERASE;
    else if(Input::isKeyJustPressed(KEY_3))
        this->usingTool = ZOOM;
}
bool TestGame::onMouseScrolled(MouseScrolledEvent& _e) {
    this->zoomLevel += _e.getScrollY();
    this->zoomLevel = engine::functions::clamp(this->zoomLevel, 1.0f, MAX_ZOOM_LEVEL);
    return true;
}

void TestGame::updateSandParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->weatherConditions[4] * _dt), -this->weatherConditions[4], this->weatherConditions[4] );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= (this->weatherConditions[4]) * 0.5f;

    Particle _tempA = *_p;

    int _width = TestGame::textureWidth;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        _tempB = this->particles[_vX + _width * _vY];
        _tempA.velocity.x = 0;
        this->writeParticle(_vX, _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);
        this->activateNeighbours(_x, _y, _width);

    } else if(this->is(_vX, _vY, ParticleType::WATER)) {
        int _vecForB = _vX + _width * _vY;
        _tempB = this->particles[_vecForB];
        _tempB.canUpdate = true;
        _tempA.velocity.x = 0;
        _tempA.velocity.y *= 0.5f;
        this->writeParticle(_vX, _vY, _vecForB, _tempA);
        this->writeParticle(_x, _y, _posInVector, _tempB);
        this->activateNeighbours(_x, _y, _width);
    } else {

        /// Down
        bool _inWater = false;
        if(this->isEmpty(_x, _y - 1) || (_inWater = this->is(_x, _y - 1, ParticleType::WATER))) {
            if(_inWater) _p->velocity.y *= 0.5f;
            else _p->velocity.y += (this->weatherConditions[4] * _dt);

            int _vecForB = _x + _width * (_y - 1);
            _tempB = this->particles[_vecForB];

            this->writeParticle(_x, _y - 1, _vecForB, _tempA);
            this->writeParticle(_x, _y, _posInVector, _tempB);
            this->activateNeighbours(_x, _y, _width);

        }
        /// Down-Left
        else if (this->isEmpty(_x - 1, _y - 1) || (_inWater = this->is(_x - 1, _y - 1, ParticleType::WATER))) {
            if(_inWater) _p->velocity.y *= 0.5f;
            else _p->velocity.y += (this->weatherConditions[4] * _dt);
                _p->velocity.x = this->random.randomi( 0, 1 ) == 0 ? -1.f : 1.f;

            int _vecForB = (_x - 1) + _width * (_y - 1);
            _tempB = this->particles[(_x - 1) + _width * (_y - 1)];

            this->writeParticle(_x - 1, _y - 1, _vecForB, _tempA);
            this->writeParticle(_x, _y, _posInVector, _tempB);
            this->activateNeighbours(_x, _y, _width);
            
        }
        /// Down-Right
        else if (this->isEmpty(_x + 1, _y - 1) || (_inWater = this->is(_x + 1, _y - 1, ParticleType::WATER))) {
            if(_inWater) _p->velocity.y *= 0.5f;
            else _p->velocity.y += (this->weatherConditions[4] * _dt);
                _p->velocity.x = this->random.randomi( 0, 1 ) == 0 ? -1.f : 1.f;

            int _vecForB = (_x + 1) + _width * (_y - 1);
            _tempB = this->particles[_vecForB];

            this->writeParticle(_x + 1, _y - 1, _vecForB, _tempA);
            this->writeParticle(_x, _y, _posInVector, _tempB);
            this->activateNeighbours(_x, _y, _width);
            
        } else {
            _p->canUpdate = false;
        }
    }
}
void TestGame::updateWaterParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    int _sign = this->random.randomi(0, 1) == 0 ? -1 : 1;
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->weatherConditions[4] * _dt), -this->weatherConditions[4], this->weatherConditions[4] );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= 0.5f;

    int _width = TestGame::textureWidth;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        int _pos = this->calcVecPos(_vX, _vY);
        _tempB = this->particles[_pos];
        this->writeParticle(_vX, _vY, *_p);
        this->writeParticle(_x, _y, _tempB);

        this->handleUnfittedDrops(_vX, _vY, _pos, _dt);
        this->activateNeighbours(_x, _y, _width);
        
    } else {

        int _below = this->calcVecPos(_x, _y - 1);

        int _firstMovement  = this->calcVecPos(_x + _sign, _y);
        int _secondMovement = this->calcVecPos(_x - _sign, _y);

        int _firstDownMovement  = this->calcVecPos(_x + _sign, _y - 1);
        int _secondDownMovement = this->calcVecPos(_x - _sign, _y - 1);

        /// Down
        if(this->isInBounds(_x, _y - 1) && this->particles[_below].type == NONE_PARTICLE) {
            _tempB = this->particles[_below];
            this->writeParticle(_x, _y - 1, *_p);
            this->writeParticle(_x, _y, _tempB);

            this->handleUnfittedDrops(_x, _y - 1, _below, _dt);
            this->activateNeighbours(_x, _y, _width);
        }

        /// Down-Right OR Down-Left
        else if (this->isInBounds(_x + _sign, _y - 1) && this->particles[_firstDownMovement].type == NONE_PARTICLE) {

            int _neighbour = 1;
            while(this->isEmpty(_x + (_sign * (_neighbour + 1)), _y - (_neighbour + 1)) && _neighbour <= 3) {
                _neighbour++;
                _firstDownMovement = this->calcVecPos(_x + (_sign * _neighbour), _y - _neighbour);
            }

            _tempB = this->particles[_firstDownMovement];
            this->writeParticle(_x + (_sign * _neighbour), _y - _neighbour, *_p);
            this->writeParticle(_x, _y, _tempB);

            this->handleUnfittedDrops(_x + (_sign * _neighbour), _y, _firstDownMovement, _dt);
            this->activateNeighbours(_x, _y, _width);
        }

        /// Down-Right OR Down-Left
        else if (this->isInBounds(_x - _sign, _y - 1) && this->particles[_secondDownMovement].type == NONE_PARTICLE) {

            int _neighbour = 1;
            while(this->isEmpty(_x - (_sign * (_neighbour + 1)), _y - (_neighbour + 1)) && _neighbour <= 3) {
                _neighbour++;
                _secondDownMovement = this->calcVecPos(_x - (_sign * _neighbour), _y - _neighbour);
            }

            _tempB = this->particles[_secondDownMovement];
            this->writeParticle(_x - (_sign * _neighbour), _y - _neighbour, *_p);
            this->writeParticle(_x, _y, _tempB);

            this->handleUnfittedDrops(_x - (_sign * _neighbour), _y, _secondDownMovement, _dt);
            this->activateNeighbours(_x, _y, _width);
        }

        /// Left OR Right
        else if (this->isEmpty(_x + _sign, _y)) {

            int _neighbour = 1;
            while(this->isEmpty(_x + (_sign * (_neighbour + 1)), _y) && _neighbour <= 3) {
                _neighbour++;
                _firstMovement = this->calcVecPos(_x + (_sign * _neighbour), _y);
            }

            _tempB = this->particles[_firstMovement];
            this->writeParticle(_x + (_sign * _neighbour), _y, *_p);
            this->writeParticle(_x, _y, _tempB);

            this->handleUnfittedDrops(_x + (_sign * _neighbour), _y, _firstMovement, _dt);
            this->activateNeighbours(_x, _y, _width);
        }

        /// Left OR Right
        else if (this->isEmpty(_x - _sign, _y)) {

            int _neighbour = 1;
            while(this->isEmpty(_x - (_sign * (_neighbour + 1)), _y) && _neighbour <= 3) {
                _neighbour++;
                _secondMovement = this->calcVecPos(_x - (_sign * _neighbour), _y);
            }

            _tempB = this->particles[_secondMovement];
            this->writeParticle(_x - (_sign * _neighbour), _y, *_p);
            this->writeParticle(_x, _y, _tempB);

            this->handleUnfittedDrops(_x - (_sign * _neighbour), _y, _secondMovement, _dt);
            this->activateNeighbours(_x, _y, _width);
            
        } else {
            _p->canUpdate = false;
        }
    }
}
void TestGame::updateStoneParticle(int _x, int _y, int _posInVector, Timestep _dt) {

}
void TestGame::updateAcidParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    int _sign = this->random.randomi(0, 1) == 0 ? -1 : 1;
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->weatherConditions[4] * _dt), -this->weatherConditions[4], this->weatherConditions[4] );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= 0.5f;

    Particle _tempA = *_p;
    ReactionInfo _ri;
    bool _reactionReallyExists = false;

    int _width = TestGame::textureWidth;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        int _pos = this->calcVecPos(_vX, _vY);
        _tempB = this->particles[_pos];
        this->writeParticle(_vX, _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);

        this->handleUnfittedDrops(_x, _y, _pos, _dt);
        this->activateNeighbours(_x, _y, _width);
    } else {

        int _below = this->calcVecPos(_x, _y - 1);

        int _firstMovement  = this->calcVecPos(_x + _sign, _y);
        int _secondMovement = this->calcVecPos(_x - _sign, _y);

        int _firstDownMovement  = this->calcVecPos(_x + _sign, _y - 1);
        int _secondDownMovement = this->calcVecPos(_x - _sign, _y - 1);

        /// Down
        if(this->isInBounds(_x, _y - 1)) {
            _tempB = this->particles[_below];
            if(this->particles[_below].type == NONE_PARTICLE) {
                this->writeParticle(_x, _y - 1, _tempA);
                this->writeParticle(_x, _y, _tempB);

                this->handleUnfittedDrops(_x, _y - 1, _below, _dt);
                this->activateNeighbours(_x, _y, _width);

                return;
            } else {
                _ri = this->reactions({_x, _y}, {_x, _y - 1}, _tempA, _tempB);
                _reactionReallyExists |= _ri.reactionExists;
                if (_ri.prob.happened) {
                    this->activateNeighbours(_x, _y, _width);
                    return;
                }
            }
        }
            /// Down-Right OR Down-Left
        if (this->isInBounds(_x + _sign, _y - 1)) {
            _tempB = this->particles[_firstDownMovement];

            if(_tempB.type == NONE_PARTICLE) {
                int _neighbour = 1;
                while(this->isEmpty(_x + (_sign * (_neighbour + 1)), _y - (_neighbour + 1)) && _neighbour < 3) {
                    _neighbour++;
                    _firstDownMovement = this->calcVecPos(_x + (_sign * _neighbour), _y - _neighbour);
                }

                _tempB = this->particles[_firstDownMovement];

                this->writeParticle(_x + (_sign * _neighbour), _y - _neighbour, _tempA);
                this->writeParticle(_x, _y, _tempB);

                this->handleUnfittedDrops(_x + (_sign * _neighbour), _y - _neighbour, _firstDownMovement, _dt);
                this->activateNeighbours(_x, _y, _width);

                return;
            } else {
                _ri = this->reactions({_x, _y}, {_x + _sign, _y - 1}, _tempA, _tempB);
                _reactionReallyExists |= _ri.reactionExists;
                if(_ri.prob.happened) {
                    this->activateNeighbours(_x, _y, _width);
                    return;
                }
            }
        }

            /// Down-Right OR Down-Left
        if (this->isInBounds(_x - _sign, _y - 1)) {
            _tempB = this->particles[_secondDownMovement];

            if(_tempB.type == NONE_PARTICLE) {
                int _neighbour = 1;
                while(this->isEmpty(_x - (_sign * (_neighbour + 1)), _y - (_neighbour + 1)) && _neighbour < 3) {
                    _neighbour++;
                    _secondDownMovement = this->calcVecPos(_x - (_sign * _neighbour), _y - _neighbour);
                }

                _tempB = this->particles[_secondDownMovement];

                this->writeParticle(_x - (_sign * _neighbour), _y - _neighbour, _tempA);
                this->writeParticle(_x, _y, _tempB);

                this->handleUnfittedDrops(_x - (_sign * _neighbour), _y - _neighbour, _secondDownMovement, _dt);
                this->activateNeighbours(_x, _y, _width);

                return;
            } else {
                _ri = this->reactions({_x, _y}, {_x - _sign, _y - 1}, _tempA, _tempB);
                _reactionReallyExists |= _ri.reactionExists;
                if(_ri.prob.happened) {
                    this->activateNeighbours(_x, _y, _width);
                    return;
                }
            }
        }

            /// Left OR Right
        if (this->isInBounds(_x + _sign, _y)) {
            _tempB = this->particles[_firstMovement];

            if(this->particles[_firstMovement].type == NONE_PARTICLE) {
                int _neighbour = 1;
                while(this->isEmpty(_x + (_sign * (_neighbour + 1)), _y) && _neighbour < 3) {
                    _neighbour++;
                    _firstMovement = this->calcVecPos(_x + (_sign * _neighbour), _y);
                }

                _tempB = this->particles[_firstMovement];

                this->writeParticle(_x + (_sign * _neighbour), _y, _tempA);
                this->writeParticle(_x, _y, _tempB);

                this->handleUnfittedDrops(_x + (_sign * _neighbour), _y, _firstMovement, _dt);
                this->activateNeighbours(_x, _y, _width);

                return;
            } else {
                _ri = this->reactions({_x, _y}, {_x + _sign, _y}, _tempA, _tempB);
                _reactionReallyExists |= _ri.reactionExists;
                if(_ri.prob.happened) {
                    this->activateNeighbours(_x, _y, _width);
                    return;
                }
            }
        }

            /// Left OR Right
        if (this->isInBounds(_x - _sign, _y)) {
            _tempB = this->particles[_secondMovement];

            if(this->particles[_secondMovement].type == NONE_PARTICLE) {
                int _neighbour = 1;
                while(this->isEmpty(_x - (_sign * (_neighbour + 1)), _y) && _neighbour < 3) {
                    _neighbour++;
                    _secondMovement = this->calcVecPos(_x - (_sign * _neighbour), _y);
                }

                _tempB = this->particles[_secondMovement];

                this->writeParticle(_x - (_sign * _neighbour), _y, _tempA);
                this->writeParticle(_x, _y, _tempB);

                this->handleUnfittedDrops(_x - (_sign * _neighbour), _y, _secondMovement, _dt);
                this->activateNeighbours(_x, _y, _width);

                return;
            } else {
                _ri = this->reactions({_x, _y}, {_x - _sign, _y}, _tempA, _tempB);
                _reactionReallyExists |= _ri.reactionExists;
                if(_ri.prob.happened) {
                    this->activateNeighbours(_x, _y, _width);
                    return;
                }
            }
        }

        if(!_reactionReallyExists)
            _p->canUpdate = false;
    }
}
void TestGame::updateDirtParticle(int _x, int _y, int _posInVector, Timestep _dt) {
    Particle* _p = &this->particles[_posInVector];
    _p->velocity.y = functions::clamp(_p->velocity.y + (this->weatherConditions[4] * _dt), -this->weatherConditions[4], this->weatherConditions[4] );

    int _vX = _x + (int)_p->velocity.x;
    int _vY = _y - (int)_p->velocity.y;

    if(!this->isEmpty(_x, _y - 1))
        _p->velocity.y *= (this->weatherConditions[4]) * 0.5f;

    Particle _tempA = *_p;

    int _width = TestGame::textureWidth;

    Particle _tempB;
    if(isEmpty(_vX, _vY)) {
        _tempB = this->particles[_vX + _width * _vY];
        _tempA.velocity.x = 0;
        this->writeParticle(_vX, _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);
        this->activateNeighbours(_x, _y, _width);

    } else {

        /// Down
        bool _inWater = false;
        if(this->isEmpty(_x, _y - 1) || (_inWater = this->is(_x, _y - 1, ParticleType::WATER))) {
            if(_inWater) _p->velocity.y *= 0.5f;
            else _p->velocity.y += (this->weatherConditions[4] * _dt);

            int _vecForB = _x + _width * (_y - 1);
            _tempB = this->particles[_vecForB];

            this->writeParticle(_x, _y - 1, _vecForB, _tempA);
            this->writeParticle(_x, _y, _posInVector, _tempB);
            this->activateNeighbours(_x, _y, _width);

        } else {
            _p->canUpdate = false;
        }
    }
}
void TestGame::handleUnfittedDrops(int _x, int _y, int _vecPos, float _dt) {
    if(this->whatToDoWithUnfittingDrops == 2)
        return;

    if (this->whatToDoWithUnfittingDrops == 0) {
        if(this->particles[_vecPos].lastHeight == _y)
            this->particles[_vecPos].lifeTimer += _dt;
        else {
            this->particles[_vecPos].lastHeight = _y;
            this->particles[_vecPos].lifeTimer = 0.0f;
        }

        if(this->particles[_vecPos].lifeTimer >= this->particles[_vecPos].lifeTime) {
            this->writeParticle(_x, _y, _vecPos, TestGame::noneParticle);
            this->drawnPixels--;
        }
    } else if (this->whatToDoWithUnfittingDrops == 1){
        LOG_INFO("EVAPORATING NOT IMPLEMENTED YET");
    }
}

Color TestGame::particleTypeToColor(const TestGame::ParticleType& _particle) {
    switch (_particle) {
        case SAND           : return this->PARTICLE_COLORS[this->random.randomi(0, 4)];
        case WATER          : return this->PARTICLE_COLORS[5];
        case STONE           : return this->PARTICLE_COLORS[this->random.randomi(6, 8)];
        case ACID           : return this->PARTICLE_COLORS[this->random.randomi(9, 10)];
        case STEAM          : return this->PARTICLE_COLORS[11];
        case SMOKE          : return this->PARTICLE_COLORS[this->random.randomi(12, 14)];
        case NONE_PARTICLE  : return this->PARTICLE_COLORS[15];
        case DIRT           : return this->PARTICLE_COLORS[this->random.randomi(16, 18)];
    }

    return this->PARTICLE_COLORS[15];
}
void TestGame::generateParticles(const Vec2f& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    if(this->particles[_posInVector].type == NONE_PARTICLE) {
        switch(this->selectedParticle) {
            case SAND   : {
                this->particles[_posInVector] = TestGame::sandParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(SAND);
                break;
            }
            case WATER  : {
                this->particles[_posInVector] = TestGame::waterParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(WATER);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                break;
            }
            case ACID   : {
                this->particles[_posInVector] = TestGame::acidParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(ACID);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                break;
            }
            case STONE   : {
                this->particles[_posInVector] = TestGame::stoneParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(STONE);
                break;
            }

            case DIRT   : {
                this->particles[_posInVector] = TestGame::dirtParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(DIRT);
                break;
            }

            default     : { LOG_ERROR("MATERIAL NOT IMPLEMENTED IN GENERATE_PARTICLES"); }
        }

        this->writeParticle((int)_mousePos.x, (int)_mousePos.y, this->particles[_posInVector]);
        this->drawnPixels++;
    }
}
void TestGame::generateWithBrush(const Vec2f& _mousePos) {

    if(this->brushSize == 1) {
        this->generateParticles(_mousePos);
    } else {
        if(this->selectedParticle != STONE) {
            std::vector<std::pair<int, int>> _spawnedPositions;
            for(int _i = 0; _i < this->brushSize; _i++) {
                auto _randPos = std::make_pair<int, int>(this->random.randomi(-this->brushSize, this->brushSize), this->random.randomi(-this->brushSize, this->brushSize));
                while(std::find(_spawnedPositions.begin(), _spawnedPositions.end(), _randPos) != _spawnedPositions.end() ||
                      (_randPos.first  + _mousePos.x < 0 || _randPos.first  + _mousePos.x >= (float)this->proceduralTexture->getWidth() - 1) ||
                      (_randPos.second + _mousePos.y < 0 || _randPos.second + _mousePos.y >= (float)this->proceduralTexture->getHeight() - 1))
                    _randPos = std::make_pair<int, int>(this->random.randomi(-this->brushSize, this->brushSize), this->random.randomi(-this->brushSize, this->brushSize));

                _spawnedPositions.emplace_back(_randPos);
                this->generateParticles({_mousePos.x + _randPos.first, _mousePos.y + _randPos.second});
            }
        } else {
            for(int _y = -this->brushSize / 2; _y < this->brushSize / 2; _y++) {
                for(int _x = -this->brushSize / 2; _x < this->brushSize / 2; _x++) {
                    if(((float)_x  + _mousePos.x >= 0 && (float)_x  + _mousePos.x <= (float)this->proceduralTexture->getWidth() - 1) &&
                       ((float)_y + _mousePos.y >= 0 && (float)_y + _mousePos.y <= (float)this->proceduralTexture->getHeight() - 1)) {
                        this->generateParticles({_mousePos.x + (float)_x, _mousePos.y + (float)_y});
                    }
                }
            }
        }
    }
}
void TestGame::zoomParticles(const Vec2f& _pos) {
    auto _io = ImGui::GetIO();
    float _spacing = 8.f;
    ImGui::BeginTooltip();
    float _toolTipHeight = ImGui::GetWindowHeight();

    float _mouseExtra[2] = {0.f, 0.f};

    float region_x = _pos.x - this->zoomImageWidth * 0.5f;
    if (region_x < 0.0f) {
        region_x = 0.0f;
        _mouseExtra[0] = (this->zoomImageWidth * 0.5f - ((_pos.x >= 0.f) ? _pos.x : 0.0f)) * this->zoomLevel;
    }
    else if (region_x > (float)textureWidth - this->zoomImageWidth) {
        region_x = (float)textureWidth - this->zoomImageWidth;
        _mouseExtra[0] = -(this->zoomImageWidth * 0.5f - ((_pos.x < (float)textureWidth) ?
                                                          (float)textureWidth - _pos.x : 1.0f)) * this->zoomLevel;
    }

    float region_y = _pos.y + this->zoomImageHeight * 0.5f;
    if (region_y < this->zoomImageHeight) {
        region_y = this->zoomImageHeight;
        _mouseExtra[1] = (this->zoomImageHeight * 0.5f - ((_pos.y >= 0.f) ? _pos.y : 0.0f)) * this->zoomLevel;
    }
    else if (region_y > (float)textureHeight) {
        region_y = (float) textureHeight;
        _mouseExtra[1] = -(this->zoomImageHeight * 0.5f - ((_pos.y < (float)textureHeight) ?
                                                           (float)textureHeight - _pos.y : 1.0f)) * this->zoomLevel;
    }

    ImGui::Text("X: %d, Y: %d", (int)_pos.x, (int)_pos.y);
    float _textHeight = ImGui::GetItemRectSize().y;

    const char* _name = "None";
    Color _color = this->backgroundColor;

    float _y = (_pos.y >= 0 ? _pos.y : 0.0f);
    _y = (_pos.y <= (float)textureHeight ? _y : (float)textureHeight);

    float _x = (_pos.x >= 0 ? _pos.x : 0.0f);
    _x = (_pos.x <= (float)textureWidth ? _x : (float)textureWidth - 1);

    if(this->isInBounds((int)_x, (int)_y)) {
        int _posVec = this->calcVecPos((int) _x, (int) _y);
        _name = this->particleTypeToName(this->particles[_posVec].type);
        _color = this->particles[_posVec].color;
    }
    ImGui::Text("Particle: %s", _name);
    _textHeight += ImGui::GetItemRectSize().y;
    ImGui::Text("R: %d, G: %d, B: %d, A: %d", _color.r, _color.g, _color.b, _color.a);
    _textHeight += ImGui::GetItemRectSize().y;

    ImVec2 uv0 = ImVec2((region_x) / (float)textureWidth, (region_y) / (float)textureHeight);
    ImVec2 uv1 = ImVec2((region_x + this->zoomImageWidth) / (float)textureWidth, (region_y - this->zoomImageHeight) / (float)textureHeight);
    ImGui::Image((void*)(intptr_t)this->proceduralTexture->getRendererID(),
                 ImVec2(this->zoomImageWidth * this->zoomLevel, this->zoomImageHeight * this->zoomLevel), uv0, uv1,
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                 ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

    float _imageWidth = ImGui::GetItemRectSize().x;
    float _imageHeight = ImGui::GetItemRectSize().y;

    const ImVec2 p = ImGui::GetCursorScreenPos();
    float x = p.x, y = p.y;

    float _topSpacing = ImGui::GetStyle().ItemSpacing.y * 3.f;
    float _yForDot = _toolTipHeight - _textHeight - ImGui::GetStyle().ItemSpacing.y * 3.f - _topSpacing;

    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + (_imageWidth / 2.f) - _mouseExtra[0],
                                                     y - _yForDot + (_imageHeight / 2.f) + _mouseExtra[1]),

                                              ImVec2(x + this->zoomLevel + (_imageWidth / 2.f) - _mouseExtra[0],
                                                     y - this->zoomLevel - _yForDot + (_imageHeight / 2.f) + _mouseExtra[1]),

                                              ImColor(ImVec4((float)this->zoomDotColor.r / 255.f, (float)this->zoomDotColor.g / 255.f, (float)this->zoomDotColor.b / 255.f, (float)this->zoomDotColor.a / 255.f)));

    ImGui::EndTooltip();
}

bool TestGame::isInBounds(int _x, int _y) {
    return _x >= 0 && _x <= (int)this->proceduralTexture->getWidth() - 1 &&
           _y >= 0 && _y <= (int)this->proceduralTexture->getHeight() - 1;
}
bool TestGame::isEmpty(int _x, int _y) {
    return this->isInBounds(_x, _y) && this->particles[this->calcVecPos(_x, _y)].type == NONE_PARTICLE;
}
bool TestGame::is(int _x, int _y, const ParticleType& _particle) {
    return this->isInBounds(_x, _y) && this->particles[this->calcVecPos(_x, _y)].type == _particle;
}
bool TestGame::isSurrounded(int _x, int _y) {
    return  this->particles[this->calcVecPos(_x - 1, _y)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x + 1, _y)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x, _y + 1)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x, _y - 1)].type != NONE_PARTICLE;
}

int TestGame::calcVecPos(int _x, int _y) {
    return _x + ((int)this->proceduralTexture->getWidth() * _y);
}
void TestGame::writeParticle(int _x, int _y, const TestGame::Particle& _particle) {
    this->particles[this->calcVecPos(_x, _y)] = _particle;
    this->proceduralTexture->setPixel(_x, _y, _particle.color);
}
void TestGame::writeParticle(int _x, int _y, int _vecPos, const TestGame::Particle& _particle) {
    this->particles[_vecPos] = _particle;
    this->proceduralTexture->setPixel(_x, _y, _particle.color);
}
void TestGame::removeParticles(const Vec2f& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    this->particles[_posInVector].type  = NONE_PARTICLE;
    this->particles[_posInVector].color = this->particleTypeToColor(NONE_PARTICLE);
}

void TestGame::imGuiAppWindow(engine::Timestep _dt) {
//    static bool _opened = true;
//    ImGui::ShowDemoWindow(&_opened);

    if (this->usingTool == ZOOM) {
        this->zoomParticles(Input::getMousePosition());
    }

    ImGui::Begin("Simulator", nullptr);
        if(this->particlesUpdateTimer >= 1.f) {
            this->particlesInSecond = this->particlesUpdating;
            this->particlesUpdateTimer = 0.0f;
        } else
            this->particlesUpdateTimer += _dt;

        if(ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen)) {
            this->imGuiInfo(_dt);
        }

        if(ImGui::CollapsingHeader("Controller")) {
            this->imGuiControllerWindow(_dt);
        }

        if(ImGui::CollapsingHeader("Conditions")) {
            this->imGuiConditions(_dt);
        }

        if(ImGui::CollapsingHeader("Weather")) {
            this->imGuiWeather(_dt);
        }

        if(ImGui::CollapsingHeader("Drawing")) {
            this->imGuiDrawingWindow(_dt);
        }

        if(ImGui::CollapsingHeader("Materials")) {
            this->imGuiMaterials(_dt);
        }

        if(ImGui::CollapsingHeader("Settings")) {
            this->imGuiSettings(_dt);
        }
    ImGui::End();
}
void TestGame::imGuiInfo(engine::Timestep _dt) {
    ImGui::Text("FPS: %d", this->app.getFps());
    ImGui::Separator();
    ImGui::Text("Updating: %d", this->particlesInSecond);
    ImGui::Separator();
    char buf[32];
    #if defined(ENGINE_PLATFORM_WINDOWS)
        sprintf_s(buf, "%d/%d", this->drawnPixels, this->totalOfPixels);
    #elif defined(ENGINE_PLATFORM_LINUX)
        sprintf(buf, "%d/%d", this->drawnPixels, this->totalOfPixels);
    #endif
    ImGui::Text("Total pixels"); ImGui::SameLine();
    ImGui::ProgressBar((float)this->drawnPixels / (float)this->totalOfPixels, ImVec2(0.f,0.f), buf);
    ImGui::Separator();
}
void TestGame::imGuiControllerWindow(engine::Timestep _dt) {
    if(ImGui::Button("Reset Scene")) {
        delete [] this->particles;
        this->initSimulationWorld();
    }

    ImGui::SameLine();
    ImGui::Text("State: %s", this->play ? "playing" : "paused");

    ImGui::Separator();
    if(ImGui::ImageButton((void*)(intptr_t)this->pauseTexture->getTexture(), ImVec2((float)this->pauseTexture->getWidth(), (float)this->pauseTexture->getHeight())))
        this->play = false;
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)(intptr_t)this->resumeTexture->getTexture(), ImVec2((float)this->resumeTexture->getWidth(), (float)this->resumeTexture->getHeight())))
        this->play = true;
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)(intptr_t)this->oneFrameTexture->getTexture(), ImVec2((float)this->oneFrameTexture->getWidth(), (float)this->oneFrameTexture->getHeight()))) {
        this->play = true;
        this->oneStep = true;
    }
    ImGui::SameLine();
    ImGui::ImageButton((void*)(intptr_t)this->advanceTexture->getTexture(), ImVec2((float)this->advanceTexture->getWidth(), (float)this->advanceTexture->getHeight()));
    if(ImGui::IsItemActive()) {
        this->play = true;
        this->oneStep = true;
    }
}
void TestGame::imGuiDrawingWindow(engine::Timestep _dt) {
    if(this->usingTool == DRAW) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if(ImGui::ImageButton((void*)(intptr_t)this->drawTexture->getTexture(), ImVec2((float)this->drawTexture->getWidth(), (float)this->drawTexture->getHeight()))) {
            this->usingTool = DRAW;
        }
        ImGui::PopStyleColor(2);
    } else {
        if(ImGui::ImageButton((void*)(intptr_t)this->drawTexture->getTexture(), ImVec2((float)this->drawTexture->getWidth(), (float)this->drawTexture->getHeight()))) {
        this->usingTool = DRAW;
    }
    }
    ImGui::SameLine();

    if(this->usingTool == ERASE) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if (ImGui::ImageButton((void*) (intptr_t) this->eraseTexture->getTexture(),ImVec2((float) this->eraseTexture->getWidth(),(float) this->eraseTexture->getHeight()))) {
            this->usingTool = ERASE;
        }
        ImGui::PopStyleColor(2);
    } else {
        if (ImGui::ImageButton((void*) (intptr_t) this->eraseTexture->getTexture(),ImVec2((float) this->eraseTexture->getWidth(),(float) this->eraseTexture->getHeight()))) {
        this->usingTool = ERASE;
    }
    }
    ImGui::SameLine();

    if(this->usingTool == ZOOM) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if (ImGui::ImageButton((void*) (intptr_t) this->zoomTexture->getTexture(),ImVec2((float) this->zoomTexture->getWidth(),(float) this->zoomTexture->getHeight()))) {
            this->usingTool = ZOOM;
        }
        ImGui::PopStyleColor(2);
    } else {
        if (ImGui::ImageButton((void*) (intptr_t) this->zoomTexture->getTexture(),ImVec2((float) this->zoomTexture->getWidth(),(float) this->zoomTexture->getHeight()))) {
            this->usingTool = ZOOM;
        }
    }
    ImGui::Separator();

    ImGui::Text("Brush thickness");
    ImGui::PushID(0);
        ImGui::SliderInt("##slider", &this->brushSize, 1, MAX_BRUSH_THICKNESS);
    ImGui::PopID();

    ImGui::Separator();

    ImGui::Text("Zoom level");
    ImGui::PushID(1);
        ImGui::SliderFloat("##slider", &this->zoomLevel, 1.f, MAX_ZOOM_LEVEL, "%.2f");
    ImGui::PopID();

    ImGui::Text("Zoomed image width");
    ImGui::PushID(2);
    ImGui::SliderFloat("##slider", &this->zoomImageWidth, 16.f, 256.f, "%.2f");
    ImGui::PopID();

    ImGui::Text("Zoomed image height");
    ImGui::PushID(3);
    ImGui::SliderFloat("##slider", &this->zoomImageHeight, 16.f, 256.f, "%.2f");
    ImGui::PopID();

    ImGui::Separator();
}
void TestGame::imGuiConditions(engine::Timestep _dt) {
    ImGui::Text("Unfitted drops");
    ImGui::Indent(30.f);
    ImGui::RadioButton("Remove", &this->whatToDoWithUnfittingDrops, 0);
    ImGui::RadioButton("Evaporate", &this->whatToDoWithUnfittingDrops, 1);
    ImGui::RadioButton("Live", &this->whatToDoWithUnfittingDrops, 2);
    ImGui::Unindent(30.f);
}
void TestGame::imGuiWeather(engine::Timestep _dt) {
    ImGui::SliderFloat("Wind",          &this->weatherConditions[0], 0, 10);
    ImGui::SliderFloat("Temperature",   &this->weatherConditions[1], -50, 200);

    ImGui::Separator();

    ImGui::SliderFloat("Rain",          &this->weatherConditions[2], 0, 10);
    const char* _liquids[] = { "Acid", "Water"};
    static const char* _liquidSelected = _liquids[1];

    ImGui::PushID(0);
    if (ImGui::BeginCombo("##combo", _liquidSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
        for (auto & _liquid : _liquids) {
            bool is_selected = (_liquidSelected == _liquid); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(_liquid, is_selected)) {
                _liquidSelected = _liquid;
                if(strcmp(_liquidSelected, "Water") == 0)
                    this->rainType = WATER;
                else if(strcmp(_liquidSelected, "Acid") == 0)
                    this->rainType = ACID;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();

    ImGui::Separator();

    ImGui::SliderFloat("Snow",          &this->weatherConditions[3], 0, 10);
    ImGui::Separator();
    ImGui::SliderFloat("Gravity",       &this->weatherConditions[4], -50, 50);
}
void TestGame::imGuiMaterials(engine::Timestep _dt) {
    static const char* _materialUsing = "Sand";

    ImGui::Text("Current: %s", _materialUsing);
    ImGui::Separator();

/// -------------------------------------- LIQUIDS

    const char* _liquids[] = { "Acid", "Water"};
    static const char* _liquidSelected = _liquids[0];

    ImGui::Text("Liquids");
    ImGui::PushID(0);
        if (ImGui::BeginCombo("##combo", _liquidSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _liquid : _liquids) {
                bool is_selected = (_liquidSelected == _liquid); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_liquid, is_selected)) {
                    _liquidSelected = _liquid;
                    if(strcmp(_liquidSelected, "Water") == 0)
                        this->selectedParticle = WATER;
                    else if(strcmp(_liquidSelected, "Acid") == 0)
                        this->selectedParticle = ACID;
                    _materialUsing = _liquid;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_liquidSelected, "Water") == 0)
                this->selectedParticle = WATER;
            else if(strcmp(_liquidSelected, "Acid") == 0)
                this->selectedParticle = ACID;
            _materialUsing = _liquidSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- DUSTS

    const char* _dusts[] = {"Gunpowder", "Salt", "Sand"};
    static const char* _dustSelected = _dusts[2];

    ImGui::Text("Dusts");
    ImGui::PushID(1);
        if (ImGui::BeginCombo("##combo", _dustSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _dust : _dusts) {
                bool is_selected = (_dustSelected == _dust); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_dust, is_selected)) {
                    _dustSelected = _dust;
                    if(strcmp(_dustSelected, "Gunpowder") == 0)
                        this->selectedParticle = GUNPOWDER;
                    else if(strcmp(_dustSelected, "Salt") == 0)
                        this->selectedParticle = SALT;
                    else if(strcmp(_dustSelected, "Sand") == 0)
                        this->selectedParticle = SAND;

                    _materialUsing = _dust;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_dustSelected, "Gunpowder") == 0)
                this->selectedParticle = GUNPOWDER;
            else if(strcmp(_dustSelected, "Salt") == 0)
                this->selectedParticle = SALT;
            else if(strcmp(_dustSelected, "Sand") == 0)
                this->selectedParticle = SAND;
            _materialUsing = _dustSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- SOLIDS

    const char* _solids[] = {"Dirt", "Ice", "Snow", "Steel","Stone", "Wax", "Wood"};
    static const char* _solidSelected = _solids[0];

    ImGui::Text("Solids");
    ImGui::PushID(2);
        if (ImGui::BeginCombo("##combo", _solidSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _solid : _solids) {
                bool is_selected = (_solidSelected == _solid); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_solid, is_selected)) {
                    _solidSelected = _solid;
                    if(strcmp(_solidSelected, "Dirt") == 0)
                        this->selectedParticle = DIRT;
                    else if(strcmp(_solidSelected, "Ice") == 0)
                        this->selectedParticle = ICE;
                    else if(strcmp(_solidSelected, "Snow") == 0)
                        this->selectedParticle = SNOW;
                    else if(strcmp(_solidSelected, "Steel") == 0)
                        this->selectedParticle = STEEL;
                    else if(strcmp(_solidSelected, "Stone") == 0)
                        this->selectedParticle = STONE;
                    else if(strcmp(_solidSelected, "Wax") == 0)
                        this->selectedParticle = WAX;
                    else if(strcmp(_solidSelected, "Wood") == 0)
                        this->selectedParticle = WOOD;

                    _materialUsing = _solid;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_solidSelected, "Dirt") == 0)
                this->selectedParticle = DIRT;
            else if(strcmp(_solidSelected, "Ice") == 0)
                this->selectedParticle = ICE;
            else if(strcmp(_solidSelected, "Snow") == 0)
                this->selectedParticle = SNOW;
            else if(strcmp(_solidSelected, "Steel") == 0)
                this->selectedParticle = STEEL;
            else if(strcmp(_solidSelected, "Stone") == 0)
                this->selectedParticle = STONE;
            else if(strcmp(_solidSelected, "Wax") == 0)
                this->selectedParticle = WAX;
            else if(strcmp(_solidSelected, "Wood") == 0)
                this->selectedParticle = WOOD;
            _materialUsing = _solidSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- GASES

    const char* _gases[] = {"Gas", "Smoke", "Steam"};
    static const char* _gasSelected = _gases[0];

    ImGui::Text("Gases");
    ImGui::PushID(3);
        if (ImGui::BeginCombo("##combo", _gasSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _gas : _gases) {
                bool is_selected = (_gasSelected == _gas); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_gas, is_selected)) {
                    _gasSelected = _gas;
                    if(strcmp(_gasSelected, "Gas") == 0)
                        this->selectedParticle = GAS;
                    else if(strcmp(_gasSelected, "Smoke") == 0)
                        this->selectedParticle = SMOKE;
                    else if(strcmp(_gasSelected, "Steam") == 0)
                        this->selectedParticle = STEAM;

                    _materialUsing = _gas;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_gasSelected, "Gas") == 0)
                this->selectedParticle = GAS;
            else if(strcmp(_gasSelected, "Smoke") == 0)
                this->selectedParticle = SMOKE;
            else if(strcmp(_gasSelected, "Steam") == 0)
                this->selectedParticle = STEAM;
            _materialUsing = _gasSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- OTHERS

    const char* others[] = {"Cloud", "Fire", "Plant"};
    static const char* _otherSelected = others[0];

    ImGui::Text("Others");
    ImGui::PushID(4);
        if (ImGui::BeginCombo("##combo", _otherSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _other : others) {
                bool is_selected = (_otherSelected == _other); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_other, is_selected)) {
                    _otherSelected = _other;
                    if(strcmp(_otherSelected, "Cloud") == 0)
                        this->selectedParticle = CLOUD;
                    else if(strcmp(_otherSelected, "Fire") == 0)
                        this->selectedParticle = FIRE;
                    else if(strcmp(_otherSelected, "Plant") == 0)
                        this->selectedParticle = PLANT;

                    _materialUsing = _other;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_otherSelected, "Cloud") == 0)
                this->selectedParticle = CLOUD;
            else if(strcmp(_otherSelected, "Fire") == 0)
                this->selectedParticle = FIRE;
            else if(strcmp(_otherSelected, "Plant") == 0)
                this->selectedParticle = PLANT;
            _materialUsing = _otherSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- USABLE

    const char* usables[] = {"Fuse"};
    static const char* _usableSelected = usables[0];

    ImGui::Text("Usable");
    ImGui::PushID(5);
        if (ImGui::BeginCombo("##combo", _usableSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _usable : usables) {
                bool is_selected = (_usableSelected == _usable); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_usable, is_selected)) {
                    _usableSelected = _usable;
                    if(strcmp(_usableSelected, "Fuse") == 0)
                        this->selectedParticle = FUSE;

                    _materialUsing = _usable;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();

        if(ImGui::Button("Select")) {
            if(strcmp(_usableSelected, "Fuse") == 0)
                this->selectedParticle = FUSE;
            _materialUsing = _usableSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

}
void TestGame::imGuiSettings(engine::Timestep _dt) {
    ImGui::Button("Save Simulation");
    ImGui::SameLine();
    ImGui::Button("Load Simulation");

    ImGui::Separator();

    static float _background[4];
    if(ImGui::CollapsingHeader("Background Color")) {
        if(ImGui::ColorPicker4("##colorPiecker4", _background, ImGuiColorEditFlags_DisplayRGB)) {
            this->backgroundColor.r = (unsigned char)(255 * _background[0]);
            this->backgroundColor.g = (unsigned char)(255 * _background[1]);
            this->backgroundColor.b = (unsigned char)(255 * _background[2]);
            this->backgroundColor.a = (unsigned char)(255);
        }
    }

    ImGui::Separator();

    static float _zoomDot[4];
    if(ImGui::CollapsingHeader("Zoom Dot Color")) {
        if(ImGui::ColorPicker4("##colorPiecker4", _zoomDot, ImGuiColorEditFlags_DisplayRGB)) {
            this->zoomDotColor.r = (unsigned char)(255 * _zoomDot[0]);
            this->zoomDotColor.g = (unsigned char)(255 * _zoomDot[1]);
            this->zoomDotColor.b = (unsigned char)(255 * _zoomDot[2]);
            this->zoomDotColor.a = (unsigned char)(255);
        }
    }
    ImGui::Separator();
}

float TestGame::probValues(const TestGame::ParticleType& _firstParticle,
                           const TestGame::ParticleType& _secondParticle) {
    if(_firstParticle == ACID) {
        if(_secondParticle == STONE)                 /// REMOVES STONE
            return 1.f / 25.f;
        else if(_secondParticle == WATER)           /// IF PROB, STEAM, ELSE HOT CONTAMINATED WATER
            return 1.f / 20.f;
    }

    return 0.0f;
}
TestGame::ReactionInfo TestGame::reactions(const Vec2i& _posA, const Vec2i& _posB, TestGame::Particle& _particleA,
                         const TestGame::Particle& _particleB) {

    ReactionInfo _ri;

    if(_particleA.type == ACID) {
        if(_particleB.type == STONE) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(TestGame::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, TestGame::noneParticle);
                this->drawnPixels--;
                if(this->random.randomf(0.0f, 1.0f) >= 0.85f) {
                    this->writeParticle(_posA.x, _posA.y, TestGame::noneParticle);
                    this->drawnPixels--;
                }
            }
        }
    }

    return _ri;
}
void TestGame::activateNeighbours(int _x, int _y, int _width) {
    if(this->isInBounds(_x, _y - 1))
        this->particles[_x + _width * (_y - 1)].canUpdate = true;

    if(this->isInBounds(_x, _y + 1))
        this->particles[_x + _width * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x - 1, _y))
        this->particles[(_x - 1) + _width * _y].canUpdate = true;

    if(this->isInBounds(_x + 1, _y))
        this->particles[(_x + 1) + _width * _y].canUpdate = true;

    if(this->isInBounds(_x - 1, _y + 1))
        this->particles[(_x - 1) + _width * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x + 1, _y + 1))
        this->particles[(_x + 1) + _width * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x - 1, _y - 1))
        this->particles[(_x - 1) + _width * (_y - 1)].canUpdate = true;

    if(this->isInBounds(_x + 1, _y - 1))
        this->particles[(_x + 1) + _width * (_y - 1)].canUpdate = true;
}

const char* TestGame::particleTypeToName(const TestGame::ParticleType& _type) {
    const char* _name;
    switch(_type) {
        case NONE_PARTICLE  : _name = "None"; return _name;
        case SAND           : _name = "Sand"; return _name;
        case GUNPOWDER      : _name = "Gunpowder"; return _name;
        case SALT           : _name = "Salt"; return _name;
        case WATER          : _name = "Water"; return _name;
        case ACID           : _name = "Acid"; return _name;
        case LAVA           : _name = "Lava"; return _name;
        case POISON_L       : _name = "Liquid Poison"; return _name;
        case STONE          : _name = "Stone"; return _name;
        case WOOD           : _name = "Wood"; return _name;
        case ICE            : _name = "Ice"; return _name;
        case SNOW           : _name = "Snow"; return _name;
        case STEEL          : _name = "Steel"; return _name;
        case WAX            : _name = "Wax"; return _name;
        case DIRT           : _name = "Dirt"; return _name;
        case STEAM          : _name = "Steam"; return _name;
        case SMOKE          : _name = "Smoke"; return _name;
        case GAS            : _name = "Gas"; return _name;
        case POISON_G       : _name = "Gas Poison"; return _name;
        case CLOUD          : _name = "Cloud"; return _name;
        case FIRE           : _name = "Fire"; return _name;
        case PLANT          : _name = "Plant"; return _name;
        case FUSE           : _name = "Fuse"; return _name;
    }
    return "Not known particle";
}

void TestGame::wind() {

}
void TestGame::rain() {
    if(this->weatherConditions[2] > 0) {
        const int SCREEN_DIVISIONS = 10;
        int _init = 0;
        int _limit = TestGame::textureWidth / SCREEN_DIVISIONS;
        for(int _i = 0; _i < SCREEN_DIVISIONS; _i++) {
            for(int _j = _init; _j < _limit; _j++) {
                if(this->random.probability(0.00025f * this->weatherConditions[2]).happened && this->isEmpty(_j, TestGame::textureHeight - 1)) {
                    int _posInVector = this->calcVecPos(_j, TestGame::textureHeight - 1);
                    switch(this->rainType) {
                        case WATER : {
                            this->particles[_posInVector] = TestGame::waterParticle;
                            this->particles[_posInVector].color = this->particleTypeToColor(WATER);
                            this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                            this->particles[_posInVector].velocity.y = 3.f;
                            break;
                        }

                        case ACID : {
                            this->particles[_posInVector] = TestGame::acidParticle;
                            this->particles[_posInVector].color = this->particleTypeToColor(ACID);
                            this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                            this->particles[_posInVector].velocity.y = 3.f;
                            break;
                        }
                    }

                    this->drawnPixels++;

                    if(_j += SCREEN_DIVISIONS / 2 < _limit)
                        _j += SCREEN_DIVISIONS / 2;
                    else
                        continue;
                }
            }

            _init += TestGame::textureWidth / SCREEN_DIVISIONS;
            _limit += TestGame::textureWidth / SCREEN_DIVISIONS;
        }
    }
}
void TestGame::snow() {

}

bool TestGame::isLiquid(int _x, int _y) {
    return false;
}

bool TestGame::isLiquid(int _posInVector) {
    return false;
}

bool TestGame::isLiquid(const TestGame::Particle& _p) {
    return false;
}



