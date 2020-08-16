#if defined (COMPILE_SAFATOR)
#include "Safator.h"

#include <fstream>

Safator::Safator() : engine::Layer("Prueba"), cameraController(false), app(Application::get()) {
    this->app.setAppIcon("assets/textures/safatorIcon.png");
}


void Safator::onInit() {
    this->app.setTitle("Safator");
    this->app.setWindowOptions(WindowOptions_Maximize | WindowOptions_Resize, false);
    this->worldTexture      = Texture2D::create((uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y, true);
    this->circleTexture     = Texture2D::create(128, 128, true);

    this->textureWidth      = this->worldTexture->getWidth();
    this->textureHeight     = this->worldTexture->getHeight();
    this->totalOfPixels     = this->textureWidth * this->textureHeight;
    this->drawnPixels       = 0;

    this->initSimulationWorld();

    this->pauseTexture      = Texture2D::create("assets/textures/pause.png");
    this->resumeTexture     = Texture2D::create("assets/textures/play-button.png");
    this->advanceTexture    = Texture2D::create("assets/textures/fast-forward.png");
    this->oneFrameTexture   = Texture2D::create("assets/textures/archivo-de-video.png");
    this->drawTexture       = Texture2D::create("assets/textures/editar.png");
    this->eraseTexture      = Texture2D::create("assets/textures/borrador.png");
    this->zoomTexture       = Texture2D::create("assets/textures/buscar.png");

    this->generateCircleTexture();

    this->frameBuffer = FrameBuffer::create({(uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y});

    LOG_WARN("Remember to add colors, create the particles on init, update solids and not updatable");
}
void Safator::onEvent(engine::Event& _e) {
    this->cameraController.onEvent(_e);

    if(_e.getEventType() == EventType::MouseScrolled) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(Safator::onMouseScrolled));
    }

}
void Safator::onUpdate(engine::Delta _dt) {
    this->checkForShortcuts();

    if(Input::isKeyJustPressed(KEY_SPACE))
        this->play = !this->play;
}
void Safator::onFixedUpdate(engine::Delta _dt) {
    if(this->play) {
        if (this->oneStep) {
            this->play = false;
            this->oneStep = false;
        }

        if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemActive() && !this->anyModalOpen) {
            auto _mousePos = Input::getMousePosition();
            if(_mousePos >= 0 && _mousePos.x < (int)this->worldTexture->getWidth() && _mousePos.y < (int)this->worldTexture->getHeight()) {
                if (Input::isMousePressed(MouseCode::Button0)) {
                    if(this->tool == DRAW)
                        this->generateWithBrush({(int)_mousePos.x, (int)_mousePos.y});
                    else if (this->tool == ERASE)
                        this->removeWithBrush({(int)_mousePos.x, (int)_mousePos.y});
                }
            }
        }

        this->rain();

        int _textureWidth = this->worldTexture->getWidth();
        /// UPDATING PARTICLES
        for (int _y = 0; _y < (int) this->worldTexture->getHeight(); _y++) {
            if (this->random.randomi(0, 1) == 0) {
                for (int _x = 0; _x < (int) this->worldTexture->getWidth(); _x++) {
                    int _pos = _x + _textureWidth * _y;
                    ParticleType _type = this->particles[_pos].type;
                    if (_type == ParticleType::NONE_PARTICLE || Safator::notUpdatable(_type) ||
                        !this->particles[_pos].canUpdate)
                        continue;
                    this->updateAllParticles(_x, _y, _pos, _type, _dt);
                }
            } else {
                for (int _x = (int) this->worldTexture->getWidth() - 1; _x > 0; _x--) {
                    int _pos = _x + _textureWidth * _y;
                    ParticleType _type = this->particles[_pos].type;
                    if (_type == ParticleType::NONE_PARTICLE || Safator::notUpdatable(_type) ||
                        !this->particles[_pos].canUpdate)
                        continue;
                    this->updateAllParticles(_x, _y, _pos, _type, _dt);
                }
            }
        }
        this->worldTexture->updateTexture();
    }
}
void Safator::onRender(engine::Delta _dt) {
    engine::Renderer::setClearColor(this->backgroundColor);
    engine::Renderer::clear();

//    engine::Renderer::beginDrawCall(this->cameraController.getCamera());
//        engine::Renderer::drawTexture({0.0f, 0.0f},
//                                      {(float) this->worldTexture->getWidth(), (float) this->worldTexture->getHeight()},
//                                      this->worldTexture);
//
//        engine::Renderer::drawTexture({(float)Input::getMouseX() - this->app.getWindowSize().x / 2.f,
//                                       (float)Input::getMouseY() - this->app.getWindowSize().y / 2.f},
//                                      {(float) this->circleTexture->getWidth(), (float) this->circleTexture->getHeight()},
//                                      this->circleTexture);
//    engine::Renderer::endDrawCall();
}
void Safator::onImGuiRender(engine::Delta _dt) {
    this->imGuiAppWindow(_dt);
    this->particlesUpdating = 0;
}
void Safator::onEnd() {
    delete this->particles;
}


void Safator::initSimulationWorld() {
    this->particles = new Particle[this->textureWidth * this->textureHeight];

    this->noneParticle.type         = NONE_PARTICLE;
    this->noneParticle.color        = this->particleTypeToColor(NONE_PARTICLE);

    for(int _y = 0; _y < this->app.getWindowSize().y; _y++) {
        for (int _x = 0; _x < this->app.getWindowSize().x; _x++) {
            int _posInVector = this->calcVecPos(_x, _y);
            this->particles[_posInVector] = this->noneParticle;
            this->worldTexture->setPixel(_x, _y, this->noneParticle.color);
        }
    }

    this->sandParticle.type         = SAND;
    this->sandParticle.color        = this->particleTypeToColor(SAND);

    this->waterParticle.type        = WATER;
    this->waterParticle.color       = this->particleTypeToColor(WATER);

    this->stoneParticle.type        = STONE;
    this->stoneParticle.color       = this->particleTypeToColor(STONE);
    this->stoneParticle.canUpdate   = false;

    this->acidParticle.type         = ACID;
    this->acidParticle.color        = this->particleTypeToColor(ACID);

    this->dirtParticle.type         = DIRT;
    this->dirtParticle.color        = this->particleTypeToColor(DIRT);

    this->iceParticle.type          = ICE;
    this->iceParticle.color         = this->particleTypeToColor(ICE);

    this->saltParticle.type         = SALT;
    this->saltParticle.color        = this->particleTypeToColor(SALT);

    this->gunpowderParticle.type    = GUNPOWDER;
    this->gunpowderParticle.color   = this->particleTypeToColor(GUNPOWDER);

    this->lavaParticle.type         = LAVA;
    this->lavaParticle.color        = this->particleTypeToColor(LAVA);

    this->obsidianParticle.type     = OBSIDIAN;
    this->obsidianParticle.color    = this->particleTypeToColor(OBSIDIAN);
    this->obsidianParticle.canUpdate= false;

    this->cryoParticle.type         = CRYOGENER;
    this->cryoParticle.color        = this->particleTypeToColor(CRYOGENER);

    this->frostParticle.type        = FROST;
    this->frostParticle.color       = this->particleTypeToColor(FROST);

    this->steamParticle.type        = STEAM;
    this->steamParticle.color       = this->particleTypeToColor(STEAM);

    this->smokeParticle.type        = SMOKE;
    this->smokeParticle.color       = this->particleTypeToColor(SMOKE);

    this->poisonGParticle.type      = POISON_G;
    this->poisonGParticle.color     = this->particleTypeToColor(POISON_G);

    this->ashParticle.type          = ASH;
    this->ashParticle.color         = this->particleTypeToColor(ASH);

    this->worldTexture->updateTexture();

}
void Safator::checkForShortcuts() {
    if(Input::isKeyJustPressed(KEY_1))
        this->tool = DRAW;
    else if(Input::isKeyJustPressed(KEY_2))
        this->tool = ERASE;
    else if(Input::isKeyJustPressed(KEY_3))
        this->tool = ZOOM;
}
bool Safator::onMouseScrolled(MouseScrolledEvent& _e) {
    if(this->tool == ZOOM) {
        this->zoomInfo.level += _e.getScrollY();
        this->zoomInfo.level = engine::functions::clamp(this->zoomInfo.level, 1.0f, MAX_ZOOM_LEVEL);
    } else if(this->tool == DRAW || this->tool == ERASE) {
        this->drawInfo.brushSize += (int)_e.getScrollY();
        this->drawInfo.brushSize = (int)engine::functions::clamp((float)this->drawInfo.brushSize, 1.0f, MAX_BRUSH_THICKNESS);

        int _lastSize = this->drawInfo.brushCircleWH;

        this->drawInfo.brushCircleWH += (int)_e.getScrollY();
        this->drawInfo.brushCircleWH = (int)engine::functions::clamp((float)this->drawInfo.brushCircleWH, 1.0f, MAX_BRUSH_THICKNESS);

        if(_lastSize != this->drawInfo.brushCircleWH)
            this->generateCircleTexture();
    }

    return true;
}


void Safator::updateAllParticles(int _x, int _y, int _posInVector, const ParticleType& _type, Delta _dt) {

    this->particlesUpdating++;
    switch (_type) {
        case SAND       :
        case GUNPOWDER  :
        case CRYOGENER  :
        case ASH        :
        case SALT       : this->updateCommonDusts(_x, _y, _posInVector, _dt);                       break;
        case DIRT       : this->updateDirtParticle(_x, _y, _posInVector, _dt);                      break;

        case WATER      : this->updateCommonLiquids(_x, _y, _posInVector, WATER_SPREAD_RATE, _dt);  break;
        case ACID       : this->updateCommonLiquids(_x, _y, _posInVector, ACID_SPREAD_RATE, _dt);   break;
        case LAVA       : this->updateCommonLiquids(_x, _y, _posInVector, LAVA_SPREAD_RATE, _dt);   break;

        case ICE        :
        case FROST      : this->updateFrostParticle(_x, _y, _posInVector, _dt);                     break;

        case SMOKE      :
        case POISON_G   :
        case STEAM      : this->updateCommonGases(_x, _y, _posInVector, _dt);                       break;
        default         :                                                                           break;
    }
}
void Safator::updateDirtParticle(int _x, int _y, int _posInVector, Delta _dt) {
    Particle* _p = &this->particles[_posInVector];

    if(_p->velocity.y < this->weatherConditions[4] * 2)
        _p->velocity.y += this->weatherConditions[4] * 2 * _dt;

    float _vX = (float)_x + _p->velocity.x;
    float _vY = (float)_y - _p->velocity.y;

    if(this->is(_x, _y - 1, WATER)) {
        _p->velocity.y *= (this->weatherConditions[4]) * 0.05f;
        _p->velocity.x = this->random.randomf( -2, 2 );
        _vX = (float)_x + _p->velocity.x;
    }

    Particle _tempA = *_p;
    Particle _tempB;

    ReactionInfo _ri;
    bool _reactionReallyExists = false;

    if(isEmpty((int)_vX, (int)_vY) || this->is((int)_vX, (int)_vY, WATER)) {
        _tempB = this->particles[(int) _vX + this->textureWidth * (int) _vY];
        this->writeParticle((int) _vX, (int) _vY, _tempA);
        this->writeParticle(_x, _y, _tempB);
        this->activateNeighbours(_x, _y);
        this->activateNeighbours((int) _vX, (int) _vY);
    }
    else {
        _p->velocity.x = (this->random.randomi(0, 1) == 0 ? 1.f : -1.f);
        _vX = ((float)_x + _p->velocity.x);

        while(_vY < (float)_y && _y > 0) {
            if(isEmpty((int)_vX, (int)_vY) || this->is((int)_vX, (int)_vY, WATER)) {
                _tempB = this->particles[(int)_vX + this->textureWidth * (int)_vY];
                this->writeParticle((int)_vX, (int)_vY, _tempA);
                this->writeParticle(_x, _y, _tempB);
                this->activateNeighbours(_x, _y);
                this->activateNeighbours((int)_vX, (int)_vY);
                return;
            } else {
                if(this->isInBounds((int)_vX, (int)_vY)) {
                    _tempB = this->particles[(int)_vX + this->textureWidth * (int)_vY];
                    _p->velocity.y = 0.f;
                    _ri = this->reactions({_x, _y}, {(int)_vX, (int)_vY}, _tempA, _tempB);
                    _reactionReallyExists |= _ri.reactionExists;
                }
            }
            _vY++;
        }

        if(!_reactionReallyExists) {
            _p->canUpdate = false;
            _p->velocity = {0.0f, 0.0f};
        }
    }
}
void Safator::updateFrostParticle(int _x, int _y, int _posInVector, Delta _dt) {
    int _pos = this->calcVecPos(_x, _y);
    Particle* _p = &this->particles[_pos];
    ReactionInfo _ri;

    bool _left  = this->isInBounds(_x - 1, _y) && !this->is(_x - 1, _y, FROST);
    bool _right = this->isInBounds(_x + 1, _y) && !this->is(_x + 1, _y, FROST);
    bool _up    = this->isInBounds(_x, _y + 1) && !this->is(_x, _y + 1, FROST);
    bool _down  = this->isInBounds(_x, _y - 1) && !this->is(_x, _y - 1, FROST);
    _p->canUpdate = _left || _right || _up || _down;

    if(_p->canUpdate) {
        Particle _tempB;
        _p->canUpdate = this->reactWithFourNeighbours(_x, _y, *_p, _tempB, _ri);
    }
}


void Safator::handleUnfittedDrops(int _x, int _y, int _vecPos, float _dt) {
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
            this->writeParticle(_x, _y, _vecPos, this->noneParticle);
            this->drawnPixels--;
        }
    } else if (this->whatToDoWithUnfittingDrops == 1){
        if(this->particles[_vecPos].lastHeight == _y)
            this->particles[_vecPos].lifeTimer += _dt;
        else {
            this->particles[_vecPos].lastHeight = _y;
            this->particles[_vecPos].lifeTimer = 0.0f;
        }

        if(this->particles[_vecPos].lifeTimer >= this->particles[_vecPos].lifeTime) {
            if(this->particles[_vecPos].type == WATER) {
                this->writeParticle(_x, _y, _vecPos, this->noneParticle);
                this->drawnPixels--;
                this->generateSpecificParticle({_x, _y}, STEAM);
            } else if(this->particles[_vecPos].type == ACID) {
                this->writeParticle(_x, _y, _vecPos, this->noneParticle);
                this->drawnPixels--;
                this->generateSpecificParticle({_x, _y}, POISON_G);
            } else if(this->particles[_vecPos].type == LAVA) {
                this->writeParticle(_x, _y, _vecPos, this->noneParticle);
                this->drawnPixels--;
                this->generateSpecificParticle({_x, _y}, SMOKE);
            }
        }
    }
}
void Safator::handleUnfittedGases(int _x, int _y, int _vecPos, float _dt) {

//    if(_y >= this->textureHeight - 1) {
//        this->writeParticle(_x, _y, _vecPos, this->noneParticle);
//        this->drawnPixels--;
//    }

    if(this->whatToDoWithUnfittingGas == 2)
        return;

    if (this->whatToDoWithUnfittingGas == 0) {
        if(this->particles[_vecPos].lastHeight == _y)
            this->particles[_vecPos].lifeTimer += _dt;
        else
            this->particles[_vecPos].lastHeight = _y;

        if(this->particles[_vecPos].lifeTimer >= this->particles[_vecPos].lifeTime) {
            this->writeParticle(_x, _y, _vecPos, this->noneParticle);
            this->drawnPixels--;
        }
    } else if (this->whatToDoWithUnfittingGas == 1){
        if(this->particles[_vecPos].lastHeight == _y)
            this->particles[_vecPos].lifeTimer += _dt;
        else
            this->particles[_vecPos].lastHeight = _y;

        if(this->particles[_vecPos].lifeTimer >= this->particles[_vecPos].lifeTime) {
            if(this->particles[_vecPos].type == STEAM) {
                this->removeParticles({_x, _y});
                this->generateSpecificParticle({_x, _y}, WATER);
            } else if(this->particles[_vecPos].type == POISON_G) {
                this->removeParticles({_x, _y});
                this->generateSpecificParticle({_x, _y}, ACID);
            } else if(this->particles[_vecPos].type == SMOKE) {
                this->removeParticles({_x, _y});
                this->generateSpecificParticle({_x, _y}, ASH);
            }
        }
    }
}


void Safator::updateCommonDusts(int _x, int _y, int _posInVector, Delta _dt) {
    Particle* _p = &this->particles[_posInVector];

    if(_p->velocity.y < this->weatherConditions[4] * 2)
        _p->velocity.y += this->weatherConditions[4] * 2 * _dt;

    float _vX = (float)_x + _p->velocity.x;
    float _vY = (float)_y - _p->velocity.y;

    if(this->is((int)_vX, (int)_vY, WATER)) {
        _p->velocity.y *= (this->weatherConditions[4]) * 0.05f;
        _p->velocity.x = this->random.randomf( -2, 2 );
    }

    ReactionInfo _ri;
    bool _reactionReallyExists = false;

    bool _canMove = false;
    int _spotsToCheckY = (_y - (int)std::abs(_vY)) > 1 ? _y - (int)std::abs(_vY) : 1;
    int _spotsToMoveY = 0, _spotsToMoveX = 0;

    for(int _spotY = 0; _spotY < _spotsToCheckY; _spotY++) {
        bool _inWater = false;
        if(this->isEmpty((int)_x, _y - _spotY - 1) || (_inWater = this->is((int)_x, _y - _spotY - 1, WATER))) {
            _canMove = true;
            _spotsToMoveY++;
            if(!_inWater)
                _p->velocity.x = 0;
            continue;
        }

        _p->velocity.x = (this->random.randomi(0, 1) ? 1.f : -1.f);

        for(int _side = 0; _side < 2; _side++) {
            if(_spotsToMoveX == 0) {
                int _nextPosX = _x + (int) _p->velocity.x;
                if (this->isEmpty(_nextPosX, _y - _spotY - 1) || this->is(_nextPosX, _y - _spotY - 1, WATER)) {
                    _spotsToMoveX++;
                    _spotsToMoveY++;
                    break;
                }
                _p->velocity.x *= -1.f;
            }
        }

        _canMove = _spotsToMoveY != 0 || _spotsToMoveX != 0;

        break;
    }

    Particle _tempB;
    if(_canMove) {
        _vY = (float)(_y - _spotsToMoveY);
        _vX = (float)_x + ((float)_spotsToMoveX * _p->velocity.x);
        _p->velocity.x = 0;

        _tempB = this->particles[(int)_vX + this->textureWidth * (int)_vY];

        this->writeParticle((int)_vX, (int)_vY, *_p);
        this->writeParticle(_x, _y, _tempB);
        this->activateNeighbours(_x, _y);
        this->activateNeighbours((int)_vX, (int)_vY);
    }
    else {
        _p->velocity.y = 0.0f;
        _reactionReallyExists = this->reactWithFourNeighbours(_x, _y, *_p, _tempB, _ri);
        if(!_reactionReallyExists)
            _p->canUpdate = false;
    }
}
void Safator::updateCommonLiquids(int _x, int _y, int _posInVector, int _spreadRate, Delta _dt) {

    Particle* _p = &this->particles[_posInVector];

    if(_p->velocity.y < this->weatherConditions[4] * 2)
        _p->velocity.y += this->weatherConditions[4] * 2 * _dt;

    float _vX = (float)_x + _p->velocity.x;
    float _vY = (float)_y - _p->velocity.y;

    Particle _tempA = *_p, _tempB;

    ReactionInfo _ri;
    bool _reactionReallyExists = false;

    bool _canMove = false;
    int _spotsToCheckY = (_y - (int)std::abs(_vY)) > 1 ? _y - (int)std::abs(_vY) : 1;
    int _spotsToMoveY = 0, _spotsToMoveX = 0;

    for(int _spotY = 0; _spotY < _spotsToCheckY; _spotY++) {
        if(this->isEmpty(_x, _y - _spotY - 1)) {
            _canMove = true;
            _spotsToMoveY++;
            continue;
        }

        if(_p->velocity.x == 0)
            _p->velocity.x = this->random.randomi(0, 1) == 0 ? 1.0f : -1.0f;

        for(int _side = 0; _side < 2; _side++) {
            if(_spotsToMoveX == 0) {
                for(int _spotX = 0; _spotX < _spreadRate; _spotX++) {
                    int _nextPosX = _x + _spotX * (int)_p->velocity.x + (int)_p->velocity.x;
                    if (this->isEmpty(_nextPosX, _y - _spotY) && _y != 0) {
                        _canMove = true;
                        _spotsToMoveX += (int) (1.f * _p->velocity.x);
                        continue;
                    }
                    _spotX = 10;
                }

                if(_spotsToMoveX == 0)
                    _p->velocity.x *= -1.f;
            }
        }

        break;
    }

    if(_canMove) {
        _vY = (float)(_y - _spotsToMoveY);
        _vX = (float)(_x + _spotsToMoveX);
        _tempB = this->particles[(int)_vX + this->textureWidth * (int)_vY];
        this->writeParticle((int)_vX, (int)_vY, *_p);
        this->writeParticle(_x, _y, _tempB);
        this->activateNeighbours(_x, _y);
        this->activateNeighbours((int)_vX, (int)_vY);
        this->handleUnfittedDrops((int)_vX, (int)_vY, this->calcVecPos((int)_vX, (int)_vY), _dt);
    }
    else {
        _p->velocity = {0.f, 0.f};
        _reactionReallyExists = this->reactWithFourNeighbours(_x, _y, *_p, _tempB, _ri);
        if(!_reactionReallyExists)
            _p->canUpdate = false;
    }
}
void Safator::updateCommonGases(int _x, int _y, int _posInVector, Delta _dt) {
    Particle* _p = &this->particles[_posInVector];

    if(!_p->updatedThisFrame) {
        _p->velocity.x = 0;
        if(_p->velocity.y < this->weatherConditions[4] * 2)
            _p->velocity.y += this->weatherConditions[4] * 2 * _dt;

        float _vX = (float)_x + _p->velocity.x;
        float _vY = (float)_y + _p->velocity.y;

        Particle _tempA = *_p, _tempB;

        ReactionInfo _ri;
        bool _reactionReallyExists = false;

        bool _canMove = false;
        int _spotsToCheckY = ((int)std::abs(_vY) - _y) > 1 ? (int)std::abs(_vY) - _y : 1;

        int _spotsToMoveY = 0, _spotsToMoveX = 0;
        float _lastVx = _vX;

        int _move = this->random.randomi(-3, 3);
        for(int _spotY = 0; _spotY < _spotsToCheckY; _spotY++) {
            if(this->isEmpty(_x, _y + _spotY + 1) || this->isEmptyForGases(_x, _y + _spotY + 1, _p->type)) {
                _canMove = true;
                _spotsToMoveY++;
//                if(this->isEmpty(_x + _move, _y + _spotsToMoveY + 1) || this->isEmptyForGases(_x + _move, _y + _spotY + 1, _p->type))
//                    _spotsToMoveX = _move;

                continue;
            }

            _p->velocity.x = this->random.randomi(0, 1) ? -1.f : 1.f;
            for(int _side = 0; _side < 2; _side++) {
                if(_spotsToMoveX == 0) {
                    for(int _spotX = 0; _spotX < WATER_SPREAD_RATE; _spotX++) {
                        int _nextPosX = _x + _spotX * (int)_p->velocity.x + (int)_p->velocity.x;
                        if (this->isEmpty(_nextPosX, _y + _spotY) || this->isEmptyForGases(_nextPosX, _y + _spotY, _p->type)) {
                            _canMove = true;
                            _spotsToMoveX += (int)_p->velocity.x;
                            continue;
                        }
                        _spotX = 100;
                    }

                    if(_spotsToMoveX == 0)
                        _p->velocity.x *= -1.f;
                }
            }

            break;
        }

        _p->updatedThisFrame = true;
        if(_canMove) {
            _vY = (float)(_y + _spotsToMoveY);
            if(_spotsToMoveX != 0)
                _vX = (float)(_x + _spotsToMoveX);
            _tempB = this->particles[(int)_vX + this->textureWidth * (int)_vY];
            this->writeParticle((int)_vX, (int)_vY, *_p);
            this->writeParticle(_x, _y, _tempB);
            this->activateNeighbours(_x, _y);
            this->activateNeighbours((int)_vX, (int)_vY);
            this->handleUnfittedGases((int)_vX, (int)_vY, this->calcVecPos((int)_vX, (int)_vY), _dt);
        } else {
            _p->velocity.y = 0.f;
            this->reactWithFourNeighbours(_x, _y, *_p, _tempB, _ri);
        }

    } else {
        _p->updatedThisFrame = false;
    }
}
bool Safator::reactWithFourNeighbours(int _x, int _y, Particle& _tempA, Particle& _tempB, Safator::ReactionInfo& _ri) {
    bool _reactionExists = false;
    if(this->isInBounds(_x, _y - 1)) {
        _ri = this->reactions({_x, _y}, {_x, _y - 1}, _tempA, this->particles[this->calcVecPos(_x, _y - 1)]);
        _reactionExists |= _ri.reactionExists;
        if(_ri.prob.happened)
            this->activateNeighbours(_x, _y);
    }

    if(this->isInBounds(_x, _y + 1)) {
        _ri = this->reactions({_x, _y}, {_x, _y + 1}, _tempA, this->particles[this->calcVecPos(_x, _y + 1)]);
        _reactionExists |= _ri.reactionExists;
        if(_ri.prob.happened)
            this->activateNeighbours(_x, _y);
    }

    if(this->isInBounds(_x - 1, _y)) {
        _ri = this->reactions({_x, _y}, {_x - 1, _y}, _tempA, this->particles[this->calcVecPos(_x - 1, _y)]);
        _reactionExists |= _ri.reactionExists;
        if(_ri.prob.happened)
            this->activateNeighbours(_x, _y);
    }

    if(this->isInBounds(_x + 1, _y)) {
        _ri = this->reactions({_x, _y}, {_x + 1, _y}, _tempA, this->particles[this->calcVecPos(_x + 1, _y)]);
        _reactionExists |= _ri.reactionExists;
        if(_ri.prob.happened)
            this->activateNeighbours(_x, _y);
    }

    return _reactionExists;
}


const char* Safator::particleTypeToName(const Safator::ParticleType& _type) {
    const char* _name;
    switch(_type) {
        case NONE_PARTICLE  : _name = "None";           return _name;
        case SAND           : _name = "Sand";           return _name;
        case GUNPOWDER      : _name = "Gunpowder";      return _name;
        case SALT           : _name = "Salt";           return _name;
        case WATER          : _name = "Water";          return _name;
        case ACID           : _name = "Acid";           return _name;
        case LAVA           : _name = "Lava";           return _name;
        case POISON_L       : _name = "Liquid Poison";  return _name;
        case STONE          : _name = "Stone";          return _name;
        case WOOD           : _name = "Wood";           return _name;
        case ICE            : _name = "Ice";            return _name;
        case SNOW           : _name = "Snow";           return _name;
        case STEEL          : _name = "Steel";          return _name;
        case WAX            : _name = "Wax";            return _name;
        case DIRT           : _name = "Dirt";           return _name;
        case STEAM          : _name = "Steam";          return _name;
        case SMOKE          : _name = "Smoke";          return _name;
        case GAS            : _name = "Gas";            return _name;
        case POISON_G       : _name = "Gas Poison";     return _name;
        case CLOUD          : _name = "Cloud";          return _name;
        case FIRE           : _name = "Fire";           return _name;
        case PLANT          : _name = "Plant";          return _name;
        case FUSE           : _name = "Fuse";           return _name;
        case CRYOGENER      : _name = "Cryogener";      return _name;
        case FROST          : _name = "Frost";          return _name;
        case OBSIDIAN       : _name = "Obsidian";       return _name;
        default             :                           return "Not known particle";
    }
}
Color Safator::particleTypeToColor(const Safator::ParticleType& _particle) {
    switch (_particle) {
        case SAND           : return this->PARTICLE_COLORS[this->random.randomi(0, 4)];
        case WATER          : return this->PARTICLE_COLORS[5];
        case STONE          : return this->PARTICLE_COLORS[this->random.randomi(6, 8)];
        case ACID           : return this->PARTICLE_COLORS[this->random.randomi(9, 10)];
        case STEAM          : return this->PARTICLE_COLORS[11];
        case SMOKE          : return this->PARTICLE_COLORS[this->random.randomi(12, 14)];
        case NONE_PARTICLE  : return this->PARTICLE_COLORS[15];
        case DIRT           : return this->PARTICLE_COLORS[this->random.randomi(16, 18)];
        case ICE            : return this->PARTICLE_COLORS[this->random.randomi(19, 20)];
        case SALT           : return this->PARTICLE_COLORS[this->random.randomi(21, 22)];
        case GUNPOWDER      : return this->PARTICLE_COLORS[this->random.randomi(23, 25)];
        case LAVA           : return this->PARTICLE_COLORS[this->random.randomi(26, 27)];
        case OBSIDIAN       : return this->PARTICLE_COLORS[this->random.randomi(28, 30)];
        case CRYOGENER      : return this->PARTICLE_COLORS[this->random.randomi(31, 32)];
        case FROST          : return this->PARTICLE_COLORS[this->random.randomi(33, 34)];
        case POISON_G       : return this->PARTICLE_COLORS[35];
        case ASH            : return this->PARTICLE_COLORS[this->random.randomi(36, 37)];
        default             : return this->PARTICLE_COLORS[15];
    }
}
void Safator::generateWithBrush(const Vec2i& _mousePos) {

    if(this->drawInfo.brushSize == 1) {
        this->generateParticles(_mousePos);
    } else {
        if(!Safator::isSolid(this->selectedParticle)) {
            std::vector<Vec2i> _spawnedPositions;
            for(int _i = 0; _i < this->drawInfo.brushSize; _i++) {
                Vec2i _randPos = this->randomPointInsideCircle({(int)_mousePos.x, (int)_mousePos.y}, this->drawInfo.brushCircleWH);

                while(std::find(_spawnedPositions.begin(), _spawnedPositions.end(), _randPos) != _spawnedPositions.end() ||
                      (_randPos.x  + _mousePos.x < 0 || (float)_randPos.x  + _mousePos.x >= (float)this->worldTexture->getWidth() - 1) ||
                      (_randPos.y + _mousePos.y < 0 || (float)_randPos.y + _mousePos.y >= (float)this->worldTexture->getHeight() - 1)) {
                    _randPos = this->randomPointInsideCircle({(int)_mousePos.x, (int)_mousePos.y}, this->drawInfo.brushCircleWH);
                }

                _spawnedPositions.emplace_back(_randPos);
                this->generateParticles({_mousePos.x + _randPos.x, _mousePos.y + _randPos.y});
            }
        } else {
            for(int _y = (int)_mousePos.y - (int)(this->circleTexture->getHeight() / 2); _y < (int)_mousePos.y + (int)(this->circleTexture->getHeight() / 2); _y++) {
                for(int _x = (int)_mousePos.x - (int)(this->circleTexture->getWidth() / 2); _x < (int)_mousePos.x + (int)(this->circleTexture->getWidth() / 2); _x++) {
                    if(this->isInBounds(_x, _y)) {
                        float dx = (float)_x - _mousePos.x;
                        float dy = (float)_y - _mousePos.y;
                        float distanceSquared = dx * dx + dy * dy;

                        if (distanceSquared < (float)(this->drawInfo.brushCircleWH * this->drawInfo.brushCircleWH))
                            this->generateParticles({_x, _y});
                    }
                }
            }
        }
    }
}
void Safator::removeWithBrush(const Vec2i& _mousePos) {
    for(int _y = (int)_mousePos.y - (int)(this->circleTexture->getHeight() / 2); _y < (int)_mousePos.y + (int)(this->circleTexture->getHeight() / 2); _y++) {
        for(int _x = (int)_mousePos.x - (int)(this->circleTexture->getWidth() / 2); _x < (int)_mousePos.x + (int)(this->circleTexture->getWidth() / 2); _x++) {
            if(this->isInBounds(_x, _y)) {
                float dx = (float)_x - _mousePos.x;
                float dy = (float)_y - _mousePos.y;
                float distanceSquared = dx * dx + dy * dy;

                if (distanceSquared < (float)(this->drawInfo.brushCircleWH * this->drawInfo.brushCircleWH) && this->particles[this->calcVecPos(_x, _y)].type != NONE_PARTICLE)
                    this->removeParticles({_x, _y});
            }
        }
    }
}
void Safator::zoomParticles(const Vec2i& _pos) {
    auto _io = ImGui::GetIO();
    float _spacing = 8.f;
    ImGui::BeginTooltip();
    float _toolTipHeight = ImGui::GetWindowHeight();

    float _mouseExtra[2] = {0.f, 0.f};

    float region_x = _pos.x - this->zoomInfo.imageWidth * 0.5f;
    if (region_x < 0.0f) {
        region_x = 0.0f;
        _mouseExtra[0] = (this->zoomInfo.imageWidth * 0.5f - ((_pos.x >= 0.f) ? _pos.x : 0.0f)) * this->zoomInfo.level;
    }
    else if (region_x > (float)textureWidth - this->zoomInfo.imageWidth) {
        region_x = (float)textureWidth - this->zoomInfo.imageWidth;
        _mouseExtra[0] = -(this->zoomInfo.imageWidth * 0.5f - ((_pos.x < (float)textureWidth) ?
                                                          (float)textureWidth - _pos.x : 1.0f)) * this->zoomInfo.level;
    }

    float region_y = _pos.y + this->zoomInfo.imageHeight * 0.5f;
    if (region_y < this->zoomInfo.imageHeight) {
        region_y = this->zoomInfo.imageHeight;
        _mouseExtra[1] = (this->zoomInfo.imageHeight * 0.5f - ((_pos.y >= 0.f) ? _pos.y : 0.0f)) * this->zoomInfo.level;
    }
    else if (region_y > (float)textureHeight) {
        region_y = (float) textureHeight;
        _mouseExtra[1] = -(this->zoomInfo.imageHeight * 0.5f - ((_pos.y < (float)textureHeight) ?
                                                           (float)textureHeight - _pos.y : 1.0f)) * this->zoomInfo.level;
    }

    ImGui::Text("X: %d, Y: %d", (int)_pos.x, (int)_pos.y);
    float _textHeight = ImGui::GetItemRectSize().y;

    const char* _name = "None";
    Color _color = this->backgroundColor;

    Vec2f _velocity = {0.f, 0.f};
    bool _updating = false;

    float _y = (_pos.y >= 0 ? _pos.y : 0.0f);
    _y = (_pos.y <= (float)textureHeight ? _y : (float)textureHeight);

    float _x = (_pos.x >= 0 ? _pos.x : 0.0f);
    _x = (_pos.x <= (float)textureWidth ? _x : (float)textureWidth - 1);

    if(this->isInBounds((int)_x, (int)_y)) {
        int _posVec = this->calcVecPos((int) _x, (int) _y);
        _name = this->particleTypeToName(this->particles[_posVec].type);
        _color = this->particles[_posVec].color;
        _updating = this->particles[_posVec].canUpdate;
        _velocity = this->particles[_posVec].velocity;
    }

    ImGui::Text("Vx: %f, Vy: %f", _velocity.x, _velocity.y);
    _textHeight += ImGui::GetItemRectSize().y;
    ImGui::Text("Updating: %s", (_updating ? "yes" : "no"));
    _textHeight += ImGui::GetItemRectSize().y;

    ImGui::Text("Particle: %s", _name);
    _textHeight += ImGui::GetItemRectSize().y;
    ImGui::Text("R: %d, G: %d, B: %d, A: %d", _color.r, _color.g, _color.b, _color.a);
    _textHeight += ImGui::GetItemRectSize().y;

    ImVec2 uv0 = ImVec2((region_x) / (float)textureWidth, (region_y) / (float)textureHeight);
    ImVec2 uv1 = ImVec2((region_x + this->zoomInfo.imageWidth) / (float)textureWidth, (region_y - this->zoomInfo.imageHeight) / (float)textureHeight);
    ImGui::Image((void*)(intptr_t)this->worldTexture->getRendererID(),
                 ImVec2(this->zoomInfo.imageWidth * this->zoomInfo.level, this->zoomInfo.imageHeight * this->zoomInfo.level), uv0, uv1,
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                 ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

    float _imageWidth = ImGui::GetItemRectSize().x;
    float _imageHeight = ImGui::GetItemRectSize().y;

    const ImVec2 p = ImGui::GetCursorScreenPos();
    float x = p.x, y = p.y;

    float _topSpacing = ImGui::GetStyle().ItemSpacing.y * 3.f;
    float _yForDot = _toolTipHeight - _textHeight - ImGui::GetStyle().ItemSpacing.y * 5.f - _topSpacing;

    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + (_imageWidth / 2.f) - _mouseExtra[0],
                                                     y - _yForDot + (_imageHeight / 2.f) + _mouseExtra[1]),

                                              ImVec2(x + this->zoomInfo.level + (_imageWidth / 2.f) - _mouseExtra[0],
                                                     y - this->zoomInfo.level - _yForDot + (_imageHeight / 2.f) + _mouseExtra[1]),

                                              ImColor(ImVec4((float)this->zoomInfo.dotColor.r / 255.f, (float)this->zoomInfo.dotColor.g / 255.f, (float)this->zoomInfo.dotColor.b / 255.f, (float)this->zoomInfo.dotColor.a / 255.f)));

    ImGui::EndTooltip();
}


void Safator::generateParticles(const Vec2i& _mousePos) {
    int _posInVector = this->calcVecPos((int)_mousePos.x, (int)_mousePos.y);
    if(this->particles[_posInVector].type == NONE_PARTICLE) {
        switch(this->selectedParticle) {
            case NONE_PARTICLE : {
                this->particles[_posInVector] = this->noneParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(NONE_PARTICLE);
                break;
            }
            case SAND       : {
                this->particles[_posInVector] = this->sandParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(SAND);
                break;
            }
            case WATER      : {
                this->particles[_posInVector] = this->waterParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(WATER);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                break;
            }
            case ACID       : {
                this->particles[_posInVector] = this->acidParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(ACID);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                break;
            }
            case STONE      : {
                this->particles[_posInVector] = this->stoneParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(STONE);
                break;
            }

            case DIRT       : {
                this->particles[_posInVector] = this->dirtParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(DIRT);
                break;
            }

            case ICE        : {
                this->particles[_posInVector] = this->iceParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(ICE);
                break;
            }

            case GUNPOWDER  : {
                this->particles[_posInVector] = this->gunpowderParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(GUNPOWDER);
                break;
            }

            case SALT       : {
                this->particles[_posInVector] = this->saltParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(SALT);
                break;
            }

            case LAVA       : {
                this->particles[_posInVector] = this->lavaParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(LAVA);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                break;
            }

            case OBSIDIAN   : {
                this->particles[_posInVector] = this->obsidianParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(OBSIDIAN);
                break;
            }

            case CRYOGENER  : {
                this->particles[_posInVector] = this->cryoParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(CRYOGENER);
                break;
            }

            case FROST      : {
                this->particles[_posInVector] = this->frostParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(FROST);
                break;
            }

            case STEAM      : {
                this->particles[_posInVector] = this->steamParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(STEAM);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_STEAM_LIFE, MAX_STEAM_LIFE);
                break;
            }

            case SMOKE      : {
                this->particles[_posInVector] = this->smokeParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(SMOKE);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_STEAM_LIFE, MAX_STEAM_LIFE);
                break;
            }

            case POISON_G   : {
                this->particles[_posInVector] = this->poisonGParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(POISON_G);
                this->particles[_posInVector].lifeTime = this->random.randomf(MIN_STEAM_LIFE, MAX_STEAM_LIFE);
                break;
            }

            case ASH  : {
                this->particles[_posInVector] = this->ashParticle;
                this->particles[_posInVector].color = this->particleTypeToColor(ASH);
                break;
            }

            default     : { LOG_ERROR("MATERIAL NOT IMPLEMENTED IN GENERATE_PARTICLES"); }
        }

        this->writeParticle(_mousePos.x, _mousePos.y, this->particles[_posInVector]);
        this->drawnPixels++;
    }
}
void Safator::generateSpecificParticle(const Vec2i& _pos, const Safator::ParticleType& _type) {
    auto _saveType = this->selectedParticle;
    this->selectedParticle = _type;
    this->generateParticles(_pos);
    this->selectedParticle = _saveType;
}
void Safator::removeParticles(const Vec2i& _mousePos) {
    this->writeParticle(_mousePos.x, _mousePos.y, this->noneParticle);
    this->activateNeighbours(_mousePos.x, _mousePos.y);
    this->drawnPixels--;
}


bool Safator::isInBounds(int _x, int _y) {
    return _x >= 0 && _x <= (int)this->worldTexture->getWidth() - 1 &&
           _y >= 0 && _y <= (int)this->worldTexture->getHeight() - 1;
}
bool Safator::isEmpty(int _x, int _y) {
    return this->isInBounds(_x, _y) && this->particles[this->calcVecPos(_x, _y)].type == NONE_PARTICLE;
}
bool Safator::isEmptyForGases(int _x, int _y, const ParticleType& _type) {
    return Safator::isInBounds(_x, _y) && !this->is(_x, _y, _type) && !Safator::isSolid(this->particles[this->calcVecPos(_x, _y)].type);
}
bool Safator::is(int _x, int _y, const ParticleType& _particle) {
    return this->isInBounds(_x, _y) && this->particles[this->calcVecPos(_x, _y)].type == _particle;
}
bool Safator::isSurrounded(int _x, int _y) {
    return  this->particles[this->calcVecPos(_x - 1, _y)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x + 1, _y)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x, _y + 1)].type != NONE_PARTICLE &&
            this->particles[this->calcVecPos(_x, _y - 1)].type != NONE_PARTICLE;
}
bool Safator::isSolid(const ParticleType& _type) {
    return _type == STONE || _type == OBSIDIAN || _type == ICE || _type == WOOD || _type == STEEL || _type == FROST;
}
bool Safator::notUpdatable(const Safator::ParticleType& _type) {
    return _type == STONE || _type == OBSIDIAN || _type == WOOD || _type == STEEL;
}


int Safator::calcVecPos(int _x, int _y) {
    return _x + ((int)this->worldTexture->getWidth() * _y);
}
void Safator::writeParticle(int _x, int _y, const Safator::Particle& _particle) {
    this->particles[this->calcVecPos(_x, _y)] = _particle;
    this->worldTexture->setPixel(_x, _y, _particle.color);
}
void Safator::writeParticle(int _x, int _y, int _vecPos, const Safator::Particle& _particle) {
    this->particles[_vecPos] = _particle;
    this->worldTexture->setPixel(_x, _y, _particle.color);
}


void Safator::imGuiAppWindow(engine::Delta _dt) {
//    static bool _opened = true;
//    ImGui::ShowDemoWindow(&_opened);

    if (this->tool == ZOOM) {
        auto _pos = Input::getMousePosition();
        this->zoomParticles({(int)_pos.x, (int)_pos.y});
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
void Safator::imGuiInfo(engine::Delta _dt) {
    if(ImGui::Button("How to use")) {
        this->openInfo = true;
        ImGui::OpenPopup("How to use");
    }

    this->imGuiHowToUse(_dt);

    ImGui::Separator();
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
    static bool _vSync = true;
    if(ImGui::Checkbox("Vsync", &_vSync))
        this->app.setVSync(_vSync);
    ImGui::Separator();
}
void Safator::imGuiHowToUse(engine::Delta _dt) {
    ImGui::SetNextWindowSize({this->app.getWindowSize().x * 0.85f, this->app.getWindowSize().y * 0.65f}, ImGuiCond_Always);
    int _width = this->textureWidth, _height = this->textureHeight;
    auto _mainWindowPos = this->app.getPosition();

    ImGui::SetNextWindowPos({(float)_mainWindowPos.x + this->app.getWindowSize().x / 2.f - this->app.getWindowSize().x * 0.85f / 2.f,
                             _mainWindowPos.y + this->app.getWindowSize().y / 2.f - this->app.getWindowSize().y * 0.65f / 2.f});

    if(ImGui::BeginPopupModal("How to use", &this->openInfo, ImGuiWindowFlags_HorizontalScrollbar)) {
        this->anyModalOpen = true;
        ImGui::TextColored({1.f, 1.f, 0.f, 1.f}, "Version: Alpha"); ImGui::SameLine(); ImGui::TextColored({0, 206.f / 255.f, 209.f / 255.f, 1.0f}, "1.1");

        ImGui::NewLine();

        ImGui::BulletText("Controls");
        ImGui::Indent(30);
            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Controller");
                ImGui::Indent();
                    ImGui::Text("Reset Scene: clears the scene.");
                    ImGui::Image((void*)(intptr_t)this->pauseTexture->getRendererID(), ImVec2((float)this->pauseTexture->getWidth() / 2.f, (float)this->pauseTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Stops the simulation.");
                    ImGui::Image((void*)(intptr_t)this->resumeTexture->getRendererID(), ImVec2((float)this->resumeTexture->getWidth() / 2.f, (float)this->resumeTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Resumes the simulation.");
                    ImGui::Image((void*)(intptr_t)this->oneFrameTexture->getRendererID(), ImVec2((float)this->oneFrameTexture->getWidth() / 2.f, (float)this->oneFrameTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Advances one frame on the simulation if it is paused.");
                    ImGui::Image((void*)(intptr_t)this->advanceTexture->getRendererID(), ImVec2((float)this->advanceTexture->getWidth() / 2.f, (float)this->advanceTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Advances as many frames as time is pressed on the simulation if it is paused.");
                ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Drawing");
                ImGui::Indent();
                    ImGui::Image((void*)(intptr_t)this->drawTexture->getRendererID(), ImVec2((float)this->drawTexture->getWidth() / 2.f, (float)this->drawTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Allows you to create particles on the simulation.");
                    ImGui::Image((void*)(intptr_t)this->eraseTexture->getRendererID(), ImVec2((float)this->eraseTexture->getWidth() / 2.f, (float)this->eraseTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Allows you to remove particles from the simulation.");
                    ImGui::Image((void*)(intptr_t)this->zoomTexture->getRendererID(), ImVec2((float)this->zoomTexture->getWidth() / 2.f, (float)this->zoomTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                    ImGui::SameLine(); ImGui::Text("Allows you to have a closer look of the simulation.");
                ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Shortcuts");
                ImGui::Indent();
                    ImGui::Text("Space: pause/resume the simulation.");
                    ImGui::Text("1: quick access to draw tool.");
                    ImGui::Text("2: quick access to erase tool.");
                    ImGui::Text("3: quick access to zoom tool.");
                ImGui::Unindent();

        ImGui::Unindent(30);

        ImGui::NewLine();

        ImGui::BulletText("Conditions");
        ImGui::Indent(30);
        ImGui::Text("When simulating, some particles may be moving forever, and can cause worse performance, so here you have some options");

        ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Unfitted drops");
                ImGui::Indent();
                    ImGui::Text("Live: they will move forever if they have to.");
                    ImGui::Text("Remove: they will be removed once certain time has passed.");
                    ImGui::Text("Evaporate: they will be evaporated once certain time has passed.");
                ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Unfitted gases");
                ImGui::Indent();
                    ImGui::Text("Live: they will move forever if they have to.");
                    ImGui::Text("Remove: they will be removed once certain time has passed.");
                    ImGui::Text("Condensate: they will be condensed once certain time has passed.");
                ImGui::Unindent();
            ImGui::Unindent(30);

        ImGui::NewLine();

        ImGui::BulletText("Weather");
        ImGui::Indent(30);
            ImGui::Text("In this update of the simulator only the rain is working, all the other options among others will be the next update.");
        ImGui::Unindent(30);

        ImGui::BulletText("Materials");
        ImGui::Indent(30);
            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Liquids");
            ImGui::Indent();
                ImGui::Text("Acid: working, reacts with stone, water, lava.");
                ImGui::Text("Water: working, reacts lava, salt and ice.");
                ImGui::Text("Lava: working, reacts with ice.");
            ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Dusts");
            ImGui::Indent();
                ImGui::Text("Ash: working, no reactions yet.");
                ImGui::Text("Dirt: working, no reactions yet.");
                ImGui::Text("Gunpowder: working, no reactions yet.");
                ImGui::Text("Salt: working, no reactions yet.");
                ImGui::Text("Sand: working.");
            ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Solids");
            ImGui::Text("They have no gravity.");
            ImGui::Indent();
                ImGui::Text("Frost: working, freezes everything.");
                ImGui::Text("Ice: working.");
                ImGui::Text("Obsidian: working, no reactions yet.");
                ImGui::Text("Snow: not implemented yet, next update.");
                ImGui::Text("Steel: not implemented yet, next update.");
                ImGui::Text("Stone: working.");
                ImGui::Text("Wax: not implemented yet, next update.");
                ImGui::Text("Wood: not implemented yet, next update.");
            ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Gases");
            ImGui::Indent();
                ImGui::Text("Gas: not implemented yet, next update.");
                ImGui::Text("Poison Gas: working, no reactions yet.");
                ImGui::Text("Smoke: working, no reactions yet.");
                ImGui::Text("Steam: working, no reactions yet.");
            ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Others");
            ImGui::Indent();
                ImGui::Text("Cloud: not implemented yet, next update.");
                ImGui::Text("Fire: not implemented yet, next update.");
                ImGui::Text("Plant: not implemented yet, next update.");
            ImGui::Unindent();

            ImGui::NewLine();

            ImGui::TextColored({1.0, 0.0f, 0.0f, 1.0f}, "Usable");
            ImGui::Indent();
                ImGui::Text("Cryogener: working, freezes everything.");
                ImGui::Text("Fuse: not implemented yet, next update.");
            ImGui::Unindent();

        ImGui::Unindent(30);

        if(ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            this->anyModalOpen = false;
        }

        ImGui::EndPopup();
    }
}
void Safator::imGuiControllerWindow(engine::Delta _dt) {
    if(ImGui::Button("Reset Scene")) {
        delete [] this->particles;
        this->drawnPixels = 0;
        this->initSimulationWorld();
    }

    ImGui::SameLine();
    ImGui::Text("State: %s", this->play ? "playing" : "paused");

    ImGui::Separator();
    if(ImGui::ImageButton((void*)(intptr_t)this->pauseTexture->getRendererID(), ImVec2((float)this->pauseTexture->getWidth() / 2.f, (float)this->pauseTexture->getHeight() / 2.f)))
        this->play = false;
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)(intptr_t)this->resumeTexture->getRendererID(), ImVec2((float)this->resumeTexture->getWidth() / 2.f, (float)this->resumeTexture->getHeight() / 2.f)))
        this->play = true;
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)(intptr_t)this->oneFrameTexture->getRendererID(), ImVec2((float)this->oneFrameTexture->getWidth() / 2.f, (float)this->oneFrameTexture->getHeight() / 2.f),ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
        this->play = true;
        this->oneStep = true;
    }
    ImGui::SameLine();
    ImGui::ImageButton((void*)(intptr_t)this->advanceTexture->getRendererID(), ImVec2((float)this->advanceTexture->getWidth() / 2.f, (float)this->advanceTexture->getHeight() / 2.f));
    if(ImGui::IsItemActive()) {
        this->play = true;
        this->oneStep = true;
    }
}
void Safator::imGuiDrawingWindow(engine::Delta _dt) {
    if(this->tool == DRAW) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if(ImGui::ImageButton((void*)(intptr_t)this->drawTexture->getRendererID(), ImVec2((float)this->drawTexture->getWidth() / 2.f, (float)this->drawTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
            this->tool = DRAW;
        }
        ImGui::PopStyleColor(2);
    } else {
        if(ImGui::ImageButton((void*)(intptr_t)this->drawTexture->getRendererID(), ImVec2((float)this->drawTexture->getWidth() / 2.f, (float)this->drawTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
        this->tool = DRAW;
    }
    }
    ImGui::SameLine();

    if(this->tool == ERASE) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if (ImGui::ImageButton((void*) (intptr_t) this->eraseTexture->getRendererID(),ImVec2((float) this->eraseTexture->getWidth() / 2.f,(float) this->eraseTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
            this->tool = ERASE;
        }
        ImGui::PopStyleColor(2);
    } else {
        if (ImGui::ImageButton((void*) (intptr_t) this->eraseTexture->getRendererID(),ImVec2((float) this->eraseTexture->getWidth() / 2.f,(float) this->eraseTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
        this->tool = ERASE;
    }
    }
    ImGui::SameLine();

    if(this->tool == ZOOM) {
        ImGui::PushStyleColor(ImGuiCol_Button, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {(float) Color::Green.r, (float) Color::Green.g, (float) Color::Green.b, (float) Color::Green.a});
        if (ImGui::ImageButton((void*) (intptr_t) this->zoomTexture->getRendererID(),ImVec2((float) this->zoomTexture->getWidth() / 2.f,(float) this->zoomTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
            this->tool = ZOOM;
        }
        ImGui::PopStyleColor(2);
    } else {
        if (ImGui::ImageButton((void*) (intptr_t) this->zoomTexture->getRendererID(),ImVec2((float) this->zoomTexture->getWidth() / 2.f,(float) this->zoomTexture->getHeight() / 2.f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })) {
            this->tool = ZOOM;
        }
    }
    ImGui::Separator();

    ImGui::Text("Brush thickness");
    ImGui::PushID(0);
        ImGui::SliderInt("##slider", &this->drawInfo.brushSize, 1, MAX_BRUSH_THICKNESS);
    ImGui::PopID();

    ImGui::Separator();

    ImGui::Text("Zoom level");
    ImGui::PushID(1);
        ImGui::SliderFloat("##slider", &this->zoomInfo.level, 1.f, MAX_ZOOM_LEVEL, "%.2f");
    ImGui::PopID();

    ImGui::Text("Zoomed image width");
    ImGui::PushID(2);
    ImGui::SliderFloat("##slider", &this->zoomInfo.imageWidth, 16.f, 256.f, "%.2f");
    ImGui::PopID();

    ImGui::Text("Zoomed image height");
    ImGui::PushID(3);
    ImGui::SliderFloat("##slider", &this->zoomInfo.imageHeight, 16.f, 256.f, "%.2f");
    ImGui::PopID();

    ImGui::Separator();
}
void Safator::imGuiConditions(engine::Delta _dt) {
    ImGui::Text("Unfitted drops");
    ImGui::Indent(30.f);
    ImGui::PushID(1);
        ImGui::RadioButton("Remove", &this->whatToDoWithUnfittingDrops, 0);
        ImGui::RadioButton("Evaporate", &this->whatToDoWithUnfittingDrops, 1);
        ImGui::RadioButton("Live", &this->whatToDoWithUnfittingDrops, 2);
    ImGui::PopID();
    ImGui::Unindent(30.f);

    ImGui::Separator();

    ImGui::Text("Unfitted Gases");
    ImGui::Indent(30.f);
    ImGui::PushID(2);
        ImGui::RadioButton("Remove", &this->whatToDoWithUnfittingGas, 0);
        ImGui::RadioButton("Condensate", &this->whatToDoWithUnfittingGas, 1);
        ImGui::RadioButton("Live", &this->whatToDoWithUnfittingGas, 2);
    ImGui::PopID();
    ImGui::Unindent(30.f);
}
void Safator::imGuiWeather(engine::Delta _dt) {
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
void Safator::imGuiMaterials(engine::Delta _dt) {
    static const char* _materialUsing = "Sand";

    ImGui::Text("Current: %s", _materialUsing);
    ImGui::Separator();

/// -------------------------------------- LIQUIDS

    const char* _liquids[] = { "Acid", "Lava", "Water"};
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
                    else if(strcmp(_liquidSelected, "Lava") == 0)
                        this->selectedParticle = LAVA;
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
            else if(strcmp(_liquidSelected, "Lava") == 0)
                this->selectedParticle = LAVA;
            _materialUsing = _liquidSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

    /// -------------------------------------- DUSTS

    const char* _dusts[] = {"Ash", "Dirt", "Gunpowder", "Salt", "Sand"};
    static const char* _dustSelected = _dusts[4];

    ImGui::Text("Dusts");
    ImGui::PushID(1);
        if (ImGui::BeginCombo("##combo", _dustSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _dust : _dusts) {
                bool is_selected = (_dustSelected == _dust); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_dust, is_selected)) {
                    _dustSelected = _dust;
                    if(strcmp(_dustSelected, "Ash") == 0)
                        this->selectedParticle = ASH;
                    else if(strcmp(_dustSelected, "Dirt") == 0)
                        this->selectedParticle = DIRT;
                    else if(strcmp(_dustSelected, "Gunpowder") == 0)
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
            if(strcmp(_dustSelected, "Ash") == 0)
                this->selectedParticle = ASH;
            else if(strcmp(_dustSelected, "Dirt") == 0)
                this->selectedParticle = DIRT;
            else if(strcmp(_dustSelected, "Gunpowder") == 0)
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

    const char* _solids[] = {"Frost", "Ice", "Obsidian", "Snow", "Steel","Stone", "Wax", "Wood"};
    static const char* _solidSelected = _solids[0];

    ImGui::Text("Solids");
    ImGui::PushID(2);
        if (ImGui::BeginCombo("##combo", _solidSelected, ImGuiComboFlags_HeightSmall)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _solid : _solids) {
                bool is_selected = (_solidSelected == _solid); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_solid, is_selected)) {
                    _solidSelected = _solid;
                    if(strcmp(_solidSelected, "Frost") == 0)
                        this->selectedParticle = FROST;
                    else if(strcmp(_solidSelected, "Ice") == 0)
                        this->selectedParticle = ICE;
                    else if(strcmp(_solidSelected, "Obsidian") == 0)
                        this->selectedParticle = OBSIDIAN;
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
            if(strcmp(_solidSelected, "Frost") == 0)
                this->selectedParticle = FROST;
            else if(strcmp(_solidSelected, "Ice") == 0)
                this->selectedParticle = ICE;
            else if(strcmp(_solidSelected, "Obsidian") == 0)
                this->selectedParticle = OBSIDIAN;
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

    const char* _gases[] = {"Gas", "Poison Gas","Smoke", "Steam"};
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
                    else if(strcmp(_gasSelected, "Poison Gas") == 0)
                        this->selectedParticle = POISON_G;
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
            else if(strcmp(_gasSelected, "Poison Gas") == 0)
                this->selectedParticle = POISON_G;
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

    const char* usables[] = {"Cryogener", "Fuse"};
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
                    else if(strcmp(_usableSelected, "Cryogener") == 0)
                        this->selectedParticle = CRYOGENER;

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
            else if(strcmp(_usableSelected, "Cryogener") == 0)
                this->selectedParticle = CRYOGENER;

            _materialUsing = _usableSelected;
        }
    ImGui::PopID();

    ImGui::Separator();

}
void Safator::imGuiSettings(engine::Delta _dt) {
    if(ImGui::Button("Save Simulation")) {
        this->saveWorld("test.safa");
    }
    ImGui::SameLine();
    if(ImGui::Button("Load Simulation")) {
        this->loadWorld("test.safa");
    }

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
            this->zoomInfo.dotColor.r = (unsigned char)(255 * _zoomDot[0]);
            this->zoomInfo.dotColor.g = (unsigned char)(255 * _zoomDot[1]);
            this->zoomInfo.dotColor.b = (unsigned char)(255 * _zoomDot[2]);
            this->zoomInfo.dotColor.a = (unsigned char)(255);
        }
    }
    ImGui::Separator();

    if(ImGui::Button("World Size")) {
        ImGui::OpenPopup("World Size Options");
    }

    this->imGuiWorldSizePopUp(_dt);

    ImGui::Separator();
}
void Safator::imGuiWorldSizePopUp(engine::Delta _dt) {
    int _width = this->textureWidth, _height = this->textureHeight;
    auto _mainWindowPos = this->app.getPosition();

    static int _futurePopWidth = 0, _futurePopHeight = 0;

    ImGui::SetNextWindowPos({(float)_mainWindowPos.x + this->app.getWindowSize().x / 2.f - (float)_futurePopWidth / 2.f,
                             _mainWindowPos.y + this->app.getWindowSize().y / 2.f - (float)_futurePopHeight / 2.f});
    if(ImGui::BeginPopupModal("World Size Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        this->anyModalOpen = true;
        _futurePopWidth = (int)ImGui::GetWindowSize().x;
        _futurePopHeight = (int)ImGui::GetWindowSize().y;
        ImGui::Text("World Width"); ImGui::SameLine(100); ImGui::InputInt("", &_width);
        ImGui::Text("World Height"); ImGui::SameLine(100); ImGui::InputInt("", &_height);

        ImGui::Separator();

        ImGui::Button("Apply"); ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            this->anyModalOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
void Safator::imGuiSaveWorld(engine::Delta _dt) {

}
void Safator::imGuiLoadWorld(engine::Delta _dt) {

}


float Safator::probValues(const Safator::ParticleType& _firstParticle, const Safator::ParticleType& _secondParticle) {
    if(_firstParticle == ACID) {
        if(_secondParticle == STONE)                 /// REMOVES STONE
            return 1.f / 25.f;
        else if(_secondParticle == WATER)           /// IF PROB, STEAM, ELSE HOT CONTAMINATED WATER
            return 1.f / 20.f;
        else if(_secondParticle == LAVA)
            return 1.f / 10.f;
    } else if(_firstParticle == WATER) {
        if(_secondParticle == ICE || _secondParticle == SALT)
            return 1.f / 200.f;
        else if(_secondParticle == ACID)
            return probValues(_secondParticle, _firstParticle);
    } else if (_firstParticle == SALT) {
        if(_secondParticle == ICE)
            return 1.f / 50.f;
    } else if(_firstParticle == ICE) {
        if(_secondParticle == WATER)
            return probValues(_secondParticle, _firstParticle);
    } else if(_firstParticle == LAVA) {
        if(_secondParticle == WATER)
            return 1.f / 5.f;
        else if(_secondParticle == ICE)
            return 1.f / 15.f;
        else if(_secondParticle == ACID)
            return probValues(_secondParticle, _firstParticle);
    } else if(_firstParticle == CRYOGENER) {
        if(_secondParticle != NONE_PARTICLE)
            return 1.f / 5.f;
    } else if(_firstParticle == FROST) {
        if(_secondParticle != NONE_PARTICLE)
            return 1.f / 10.f;
    }

    return 0.0f;
}
Safator::ReactionInfo Safator::reactions(const Vec2i& _posA, const Vec2i& _posB, Safator::Particle& _particleA, Safator::Particle& _particleB) {

    ReactionInfo _ri;

    if(_particleA.type == _particleB.type)
        return _ri;

    if(_particleB.type == FROST && _particleA.type != CRYOGENER)
        return this->reactions(_posB, _posA, _particleB, _particleA);

    if(_particleB.type == CRYOGENER)
        return this->reactions(_posB, _posA, _particleB, _particleA);

    if(_particleA.type == ACID) {
        if(_particleB.type == STONE) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, this->noneParticle);
                this->drawnPixels--;
                if(this->random.probability(0.15f).happened) {
                    this->writeParticle(_posA.x, _posA.y, this->noneParticle);
                    this->drawnPixels--;
                }
            }
        } else if(_particleB.type == WATER) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, this->noneParticle);
                this->generateSpecificParticle({_posB.x, _posB.y}, STEAM);
                this->drawnPixels--;
                if(this->random.probability(0.35f).happened) {
                    this->writeParticle(_posA.x, _posA.y, this->noneParticle);
                    this->drawnPixels--;
                }
            }
        } else if(_particleB.type == LAVA) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->removeParticles({_posA.x, _posA.y});
                this->generateSpecificParticle({_posA.x, _posA.y}, POISON_G);
            }
        }
    }

    else if(_particleA.type == WATER) {
        if(_particleB.type == ICE) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posA.x, _posA.y, this->iceParticle);
            }
        } else if(_particleB.type == SALT) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, this->waterParticle);
            }
        } else if(_particleB.type == LAVA || _particleB.type == ACID) {
            return this->reactions(_posB, _posA, _particleB, _particleA);
        }
    }

    else if(_particleA.type == SALT) {
        if(_particleB.type == ICE) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, this->noneParticle);
                this->generateSpecificParticle({_posB.x, _posB.y}, WATER);
            }
        } else if(_particleB.type == WATER)
            return this->reactions(_posB, _posA, _particleB, _particleA);
    }

    else if(_particleA.type == ICE) {
        if(_particleB.type == WATER || _particleB.type == SALT || _particleB.type == LAVA)
            return this->reactions(_posB, _posA, _particleB, _particleA);
    }

    else if(_particleA.type == LAVA) {
        if(_particleB.type == WATER) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                if(this->random.probability(0.85f).happened) {
                    this->removeParticles({_posB.x, _posB.y});
                    this->generateSpecificParticle({_posB.x, _posB.y}, STEAM);
                } else {
                    this->removeParticles({_posB.x, _posB.y});
                    this->removeParticles({_posA.x, _posA.y});
                    this->generateSpecificParticle({_posA.x, _posA.y}, OBSIDIAN);
                    this->generateSpecificParticle({_posB.x, _posB.y}, STEAM);
                }
            }
        }
        else if(_particleB.type == ICE) {
            _ri.reactionExists = true;
            if((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->removeParticles({_posB.x, _posB.y});
                this->generateSpecificParticle({_posB.x, _posB.y}, WATER);
            }
        }
        else if(_particleB.type == ACID)
            return this->reactions(_posB, _posA, _particleB, _particleA);
    }

    else if(_particleA.type == CRYOGENER) {
        if(_particleB.type != FROST) {
            _ri.reactionExists = true;
            if ((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->removeParticles({_posB.x, _posB.y});
                this->generateSpecificParticle({_posB.x, _posB.y}, FROST);
            }
        }
    }

    else if(_particleA.type == FROST) {
        if(_particleB.type != NONE_PARTICLE && _particleB.type != CRYOGENER) {
            _ri.reactionExists = true;
            if ((_ri.prob = this->random.probability(Safator::probValues(_particleA.type, _particleB.type))).happened) {
                this->writeParticle(_posB.x, _posB.y, this->noneParticle);
                this->drawnPixels--;
                this->generateSpecificParticle({_posB.x, _posB.y}, FROST);
            }
        }
    }

    return _ri;
}
void Safator::activateNeighbours(int _x, int _y) {
    if(this->isInBounds(_x, _y - 1))
        this->particles[_x + this->textureWidth * (_y - 1)].canUpdate = true;

    if(this->isInBounds(_x, _y + 1))
        this->particles[_x + this->textureWidth * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x - 1, _y))
        this->particles[(_x - 1) + this->textureWidth * _y].canUpdate = true;

    if(this->isInBounds(_x + 1, _y))
        this->particles[(_x + 1) + this->textureWidth * _y].canUpdate = true;

    if(this->isInBounds(_x - 1, _y + 1))
        this->particles[(_x - 1) + this->textureWidth * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x + 1, _y + 1))
        this->particles[(_x + 1) + this->textureWidth * (_y + 1)].canUpdate = true;

    if(this->isInBounds(_x - 1, _y - 1))
        this->particles[(_x - 1) + this->textureWidth * (_y - 1)].canUpdate = true;

    if(this->isInBounds(_x + 1, _y - 1))
        this->particles[(_x + 1) + this->textureWidth * (_y - 1)].canUpdate = true;
}


void Safator::wind() {

}
void Safator::rain() {
    if(this->weatherConditions[2] > 0) {
        const int SCREEN_DIVISIONS = 10;
        int _init = 0;
        int _limit = this->textureWidth / SCREEN_DIVISIONS;
        for(int _i = 0; _i < SCREEN_DIVISIONS; _i++) {
            for(int _j = _init; _j < _limit; _j++) {
                if(this->random.probability(0.00025f * this->weatherConditions[2]).happened && this->isEmpty(_j, this->textureHeight - 1)) {
                    int _posInVector = this->calcVecPos(_j, this->textureHeight - 1);
                    switch(this->rainType) {
                        case WATER : {
                            this->particles[_posInVector] = this->waterParticle;
                            this->particles[_posInVector].color = this->particleTypeToColor(WATER);
                            this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                            this->particles[_posInVector].velocity.y = 3.f;
                            break;
                        }

                        case ACID : {
                            this->particles[_posInVector] = this->acidParticle;
                            this->particles[_posInVector].color = this->particleTypeToColor(ACID);
                            this->particles[_posInVector].lifeTime = this->random.randomf(MIN_WATER_LIFE, MAX_WATER_LIFE);
                            this->particles[_posInVector].velocity.y = 3.f;
                            break;
                        }

                        default: ;
                    }

                    this->drawnPixels++;

                    if(_j += SCREEN_DIVISIONS / 2 < _limit)
                        _j += SCREEN_DIVISIONS / 2;
                    else
                        continue;
                }
            }

            _init += this->textureWidth / SCREEN_DIVISIONS;
            _limit += this->textureWidth / SCREEN_DIVISIONS;
        }
    }
}
void Safator::snow() {

}


void Safator::generateCircleTexture() {
    for(int _i = 0; _i < (int)this->circleTexture->getWidth(); _i++)
        for(int _j = 0; _j < (int)this->circleTexture->getHeight(); _j++)
            this->circleTexture->setPixel(_i, _j, Color::Transparent);

    this->circleMidPoints({(int)(this->circleTexture->getWidth() / 2), (int)(this->circleTexture->getHeight() / 2)}, this->drawInfo.brushCircleWH, Color::White);
    this->circleTexture->updateTexture();
}
void Safator::circleMidPoints(const Vec2i& _center, int _radius, const Color& _color) {
    int _x = 0;
    int _y = _radius - 1;
    int _p = (5 - _radius * 4) / 4;
    circlePoints(_center, {_x, _y}, _color);
    while (_x < _y) {
        _x++;
        if (_p < 0) {
            _p += 2 * _x + 1;
        } else {
            _y--;
            _p += 2 * (_x - _y) + 1;
        }
        circlePoints(_center, {_x, _y}, _color);
    }
}
void Safator::circlePoints(const Vec2i& _center, const Vec2i& _pos, const Color& _color) {
    if (_pos.x == 0) {
        this->circleTexture->setPixel(_center.x, _center.y + _pos.y, _color);
        this->circleTexture->setPixel(_center.x, _center.y - _pos.y, _color);
        this->circleTexture->setPixel(_center.x + _pos.y, _center.y, _color);
        this->circleTexture->setPixel(_center.x - _pos.y, _center.y, _color);
    } else if (_pos.x == _pos.y) {
        this->circleTexture->setPixel(_center.x + _pos.x, _center.y + _pos.y, _color);
        this->circleTexture->setPixel(_center.x - _pos.x, _center.y + _pos.y, _color);
        this->circleTexture->setPixel(_center.x + _pos.x, _center.y - _pos.y, _color);
        this->circleTexture->setPixel(_center.x - _pos.x, _center.y - _pos.y, _color);
    } else if (_pos.x < _pos.y) {
        this->circleTexture->setPixel(_center.x + _pos.x, _center.y + _pos.y, _color);
        this->circleTexture->setPixel(_center.x - _pos.x, _center.y + _pos.y, _color);
        this->circleTexture->setPixel(_center.x + _pos.x, _center.y - _pos.y, _color);
        this->circleTexture->setPixel(_center.x - _pos.x, _center.y - _pos.y, _color);
        this->circleTexture->setPixel(_center.x + _pos.y, _center.y + _pos.x, _color);
        this->circleTexture->setPixel(_center.x - _pos.y, _center.y + _pos.x, _color);
        this->circleTexture->setPixel(_center.x + _pos.y, _center.y - _pos.x, _color);
        this->circleTexture->setPixel(_center.x - _pos.y, _center.y - _pos.x, _color);
    }
}
Vec2i Safator::randomPointInsideCircle(const Vec2i& _mousePos, int _radius) {
    float _a = this->random.randomf(0, 1);
    float _b = this->random.randomf(0, 1);
    return {(int)(_b * (float)_radius * cos( 2 * PI * _a / _b)), (int)(_b * (float)_radius * sin(2 * PI * _a / _b))};
}

void Safator::saveWorld(const std::string& _worldName) {
    std::ofstream _outputFile("maps/test.safa", std::ios::out);
    if(!_outputFile)
        LOG_ERROR("Couldn't save file");
    else {
        MapInfo _mapInfo{};
        _mapInfo.width = (uint32_t)this->app.getWindowSize().x;
        _mapInfo.height = (uint32_t)this->app.getWindowSize().y;
        _mapInfo.numberOfComponents = (uint32_t)4;
        _mapInfo.pixels = this->worldTexture->getBuffer();

        _outputFile.write((char*) &_mapInfo.width, sizeof(uint32_t));
        _outputFile.write((char*) &_mapInfo.height, sizeof(uint32_t));
        _outputFile.write((char*) &_mapInfo.numberOfComponents, sizeof(uint32_t));
        for(int _i = 0; _i < this->app.getWindowSize().x * this->app.getWindowSize().y; _i++)
            _outputFile.write((char*) &this->particles[_i], sizeof(Particle));
        _outputFile.close();
    }
}
void Safator::loadWorld(const std::string& _worldName) {
    std::ifstream _inputFile("maps/test.safa", std::ios::out | std::ios::binary);
    if(!_inputFile)
        LOG_ERROR("Couldn't load file");

    MapInfo _mapInfo{};


    _inputFile.read((char*) &_mapInfo.width, sizeof(uint32_t));
    _inputFile.read((char*) &_mapInfo.height, sizeof(uint32_t));
    _inputFile.read((char*) &_mapInfo.numberOfComponents, sizeof(uint32_t));

    _inputFile.read((char*)_mapInfo.pixels, _mapInfo.width * _mapInfo.height * _mapInfo.numberOfComponents);
    _inputFile.close();

    this->worldTexture->setBuffer(_mapInfo.pixels);

//    delete [] _buffer;
//    const int _size = (int)_width * (int)_height * 4;
//    unsigned char _data[1024];
//
//    _inputFile.read((char*) &_data, _width * _height * 4);
//    this->worldTexture->setData(_data, sizeof(unsigned char));

    LOG_INFO("Loaded, width: {0}, height: {1}", _mapInfo.width, _mapInfo.height);
}


#endif