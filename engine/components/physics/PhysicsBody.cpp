#include "PhysicsBody.h"
#include <engine/gameObject/GameObject.h>

namespace engine {

    PhysicsBodyPtr PhysicsBody::create(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType, const Vec2f& _gravity, const MovementType& _movementType) {
        return std::make_shared<PhysicsBody>(_gameObject, _position, _bodyType, _gravity, _movementType);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    PhysicsBody::PhysicsBody(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType, const Vec2f& _gravity, const MovementType& _movementType) :
    Component(_gameObject, ComponentType::PHYSICS_BODY), position(_position), gravity(_gravity), bodyType(_bodyType), movementType(_movementType) {
        this->goalVelocity = {0, 0};
        this->velocity = {0, 0};
        this->angularSpeed = 50;
        this->gravity = {0, -5};
    }

    void PhysicsBody::move(float _dt) {
        if(this->movementType == MovementType::LINEAR) {
            this->position.x += this->velocity.x * _dt;
            this->position.y += this->velocity.y * _dt;
        } else if(this->movementType == MovementType::APPROACH) {
            this->velocity.x = this->approach(this->goalVelocity.x, this->velocity.x, _dt);
            this->velocity.y = this->approach(this->goalVelocity.y, this->velocity.y, _dt);

            this->position += this->velocity * _dt;
        } else {
            this->velocity += this->acceleration * _dt;
            this->position += this->velocity * _dt;
        }
    }

    void PhysicsBody::applyGravity(float _dt) {
        this->position += this->velocity * _dt;
        if(this->velocity.y > MAX_VELOCITY_Y) this->velocity += (this->gravity * this->gravity / 2.f) * -1 * _dt;

//        LOG_INFO_CORE("Velocity[y]: {0}", this->velocity.y);
    }

    void PhysicsBody::update(float _dt) {

    }

    void PhysicsBody::fixUpdate(float _dt) {
        if(this->bodyType == BodyType::STATIC) return;
        else if (this->bodyType == BodyType::DYNAMIC) {
            if(!this->grounded)
                this->applyGravity(_dt);
            this->move(_dt);
        }
        this->gameObject->transform.position = this->position;
    }

    void PhysicsBody::postUpdate(float _dt) {

    }

    float PhysicsBody::approach(float _goalVelocity, float _currentVelocity, float _dt) {
        float _difference = _goalVelocity - _currentVelocity;
        if(_difference > _dt) return _currentVelocity + _dt;
        if(_difference < -_dt) return _currentVelocity - _dt;
        return _goalVelocity;
    }
}