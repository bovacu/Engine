#include "World.h"


namespace engine {

    World::World(float _timeStep, const Vec2f& _gravity) : timeStep(_timeStep), gravity(_gravity) {  }

    void World::init() {

    }

    void World::update(Timestep _dt) {
        for (auto& _object : this->objects)
            _object->getComponentOfType<PhysicsBody>()->update(_dt);
    }

    void World::fixedUpdate(Timestep _fixDt) {
        for(auto& _object : this->objects)
            _object->getComponentOfType<PhysicsBody>()->fixUpdate(_fixDt);

        for(auto& _object : this->objects) {
            auto _colliders = _object->getComponentsOfType<Collider>();
            for (auto& _collider : _colliders) {
                _collider->update(_fixDt);
                _collider->manifolds.clear();
            }
        }

        this->detectCollisions();
    }

    void World::detectCollisions() {
        for (auto& _objectA : this->objects) {
            auto _collidersA = _objectA->getComponentsOfType<Collider>();
            auto _bodyA = _objectA->getComponentOfType<PhysicsBody>();
            _bodyA->grounded = false;
            for (auto& _objectB : this->objects) {
                auto _collidersB = _objectB->getComponentsOfType<Collider>();

                for (auto &_colliderA : _collidersA) {
                    for (auto &_colliderB : _collidersB) {
                        if (_colliderA->gameObject != _colliderB->gameObject && !_colliderA->isGhost &&
                            !_colliderB->isGhost) {
                            if (_bodyA->bodyType != BodyType::STATIC) {
                                if (!std::any_of(_colliderB->manifolds.begin(), _colliderB->manifolds.end(),
                                                 [&](Manifold& _manifold) {
                                                     return _manifold.otherCollider == _colliderA.get();
                                                 })) {
                                    Mtv _mtv = _colliderA->collides(*_colliderB);
                                    _bodyA->grounded |= _mtv.collision;
                                    if (_mtv.collision) {
                                        _colliderA->manifolds.emplace_back(Manifold(_colliderB.get(), _mtv));
                                        _colliderB->manifolds.emplace_back(Manifold(_colliderA.get(), _mtv));
                                        _colliderA->gameObject->transform.position += _mtv.translation;
                                        _colliderA->setPosition(_colliderA->gameObject->transform.position);
                                        _bodyA->position = _colliderA->gameObject->transform.position;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(_bodyA->grounded)
                _bodyA->velocity.y = 0;
        }
    }

    bool World::addGameObject(GameObjectPtr& _gameObject, bool _isStatic) {
        auto _physicsBody = _gameObject->getComponentOfType<PhysicsBody>();

        if(_physicsBody == nullptr) {
            if(_isStatic) {
                _physicsBody = _gameObject->addPhysicsBody();
            } else
                return false;
        }

        if(std::find(this->objects.begin(), this->objects.end(), _gameObject) != this->objects.end())
            return false;

        this->objects.emplace_back(_gameObject);
        return true;
    }

    bool World::removeGameObject(GameObjectPtr& _gameObject) {
        auto _toRemove = std::find(this->objects.begin(), this->objects.end(), _gameObject);
        if(_toRemove != this->objects.end()) {
            this->objects.erase(_toRemove);
            return true;
        }

        return false;
//        auto _objectIt = this->objects.begin();
//
//        while(_objectIt != this->objects.end()) {
//            if(_objectIt->get() == _gameObject.get()) {
//                _objectIt = this->objects.erase(_objectIt);
//                return true;
//            }
//            ++_objectIt;
//        }
//
//        return false;
    }

    void World::clearWorld() {
        this->objects.clear();
    }

    void World::debugWorld(const OrthographicCamera& _camera) {
        if(this->debuggingFunction != nullptr) {
//            this->debuggingFunction(_batch);
        } else {
            Render2D::beginDraw(_camera);
                for(auto& _object : this->objects) {
                    auto _colliders = _object->getComponentsOfType<Collider>();
                    for(auto& _collider : _colliders) {
                        if(_collider->componentType == ComponentType::CIRCLE_COLLIDER) {
    //                        float _radius = _collider->size.width;
    //                        _batch.drawCircle(_collider->center - _radius, _radius);
                        } else
                            Render2D::drawRect(_collider->gameObject->transform.position, _collider->size, {0, 255, 0, 75});
                    }
                }
            Render2D::endDraw();
        }
    }

    void World::setDebuggingFunction(const World::DebuggingFunction &_debuggingFunction) {

    }

}