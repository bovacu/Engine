#include "GameObject.h"

namespace engine {

    GameObjectPtr GameObject::create(const Transform2D& _transform) {
        return std::make_shared<GameObject>(_transform);
    }

    GameObjectPtr GameObject::create(const Vec2f &_position, float _rotation, const Vec2f& _scale) {
        return std::make_shared<GameObject>(_position, _rotation, _scale);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    GameObject::GameObject() {  }

    GameObject::GameObject(const Transform2D& _transform) : transform(_transform) {  }

    GameObject::GameObject(const Vec2f& _position, float _rotation, const Vec2f& _scale) : transform(_position, _rotation, _scale) {  }

    ComponentPtr GameObject::addComponent(const ComponentPtr& _component) {
        _component->gameObject = this;
        this->components.emplace_back(_component);
        return _component;
    }

    ComponentPtr GameObject::removeComponent(const ComponentPtr& _component) {
        LOG_CRITICAL_CORE("REMOVE FROM GAME_OBJECT IS NOT FUNCTIONAL YET");
        return nullptr;
    }

    BoxColliderPtr GameObject::addBoxCollider(const Size& _size, bool _isGhost) {
        auto _boxCollider = BoxCollider::create(this, this->transform.position, _size, _isGhost);
        this->components.emplace_back(_boxCollider);
        return _boxCollider;
    }

    CircleColliderPtr GameObject::addCircleCollider(float _radius, bool _isGhost) {
        auto _circleCollider = CircleCollider::create(this, this->transform.position, _radius, _isGhost);
        this->components.emplace_back(_circleCollider);
        return _circleCollider;
    }

    PolygonColliderPtr GameObject::addPolygonCollider(const std::vector<Vec2f>& _vertices, bool _isGhost) {
        auto _polygonCollider = PolygonCollider::create(this, this->transform.position, _vertices, _isGhost);
        this->components.emplace_back(_polygonCollider);
        return _polygonCollider;
    }

    PhysicsBodyPtr GameObject::addPhysicsBody() {
        auto _physicsBody = PhysicsBody::create(this, this->transform.position);
        this->components.emplace_back(_physicsBody);
        return _physicsBody;
    }

    SpritePtr GameObject::addSprite(const TextureRegionPtr& _texture) {
        auto _sprite = Sprite::create(this, _texture);
        this->components.emplace_back(_sprite);
        return _sprite;
    }

    void GameObject::update(Timestep _dt) {
        for(auto& _component : this->components)
            _component->update(_dt);
    }

    void GameObject::fixUpdate(Timestep _dt) {
        for(auto& _component : this->components)
            _component->fixUpdate(_dt);
    }


}