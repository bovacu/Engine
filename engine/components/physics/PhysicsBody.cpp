#include "PhysicsBody.h"
#include <engine/gameObject/GameObject.h>

namespace engine {

    PhysicsBody* PhysicsBody::create(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType, const Vec2f& _gravity, const MovementType& _movementType) {
        return std::make_shared<PhysicsBody>(_gameObject, _position, _bodyType, _gravity, _movementType).get();
    }

    /// ----------------------------------------------------------------------------------------------------------------

    PhysicsBody::PhysicsBody(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType, const Vec2f& _gravity, const MovementType& _movementType) :
    Component(_gameObject, ComponentType::PHYSICS_BODY), position(_position), gravity(_gravity), bodyType(_bodyType), movementType(_movementType) {
        this->angularSpeed = 50;
    }

    void PhysicsBody::move(float _dt) {
        if(this->bodyType == BodyType::STATIC) return;
        else if (this->bodyType == BodyType::DYNAMIC) {
            if(this->movementType == MovementType::LINEAR) {
                this->position.x += this->velocity.x * _dt;
                this->position.y += this->velocity.y * _dt;
            } else if(this->movementType == MovementType::APPROACH) {
                this->velocity.x = this->approach(this->gaolVelocity.x, this->velocity.x, _dt * 65);
                this->velocity.y = this->approach(this->gaolVelocity.y, this->velocity.y, _dt * 65);

                this->position += this->velocity * _dt * 65;
            } else {
                this->velocity += this->acceleration * _dt;
                this->position += this->velocity * _dt;
            }
        }

        this->gameObject->transform.position = this->position;
    }

    void PhysicsBody::applyGravity(float _dt) {

    }

    void PhysicsBody::update(float _dt) {
        this->move(_dt);
    }

    void PhysicsBody::fixUpdate(float _dt) {

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