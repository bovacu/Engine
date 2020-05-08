#include "GameObject.h"

namespace engine {

    GameObject* GameObject::create(const Transform2D& _transform) {
        return std::make_shared<GameObject>(_transform).get();
    }

    GameObject* GameObject::create(const Vec2f &_position, float _rotation, const Vec2f& _scale) {
        return std::make_shared<GameObject>(_position, _rotation, _scale).get();
    }

    /// ----------------------------------------------------------------------------------------------------------------

    GameObject::GameObject() {  }

    GameObject::GameObject(const Transform2D& _transform) : transform(_transform) {  }

    GameObject::GameObject(const Vec2f& _position, float _rotation, const Vec2f& _scale) : transform(_position, _rotation, _scale) {  }

    Ptr::ComponentPtr GameObject::addComponent(Ptr::ComponentPtr _component) {
        _component->gameObject = this;
        this->components.emplace_back(_component);
        return _component;
    }

    Ptr::BoxColliderPtr GameObject::addBoxCollider(const Size& _size, bool _isGhost) {
        auto _boxCollider = BoxCollider::create(this, this->transform.position, _size, _isGhost);
        this->components.emplace_back(_boxCollider);
        return _boxCollider;
    }

    Ptr::CircleColliderPtr GameObject::addCircleCollider(float _radius, bool _isGhost) {
        Ptr::CircleColliderPtr _circleCollider = std::make_shared<CircleCollider>(this, this->transform.position, _radius, _isGhost);
        this->components.emplace_back(_circleCollider);
        return _circleCollider;
    }

    Ptr::PolygonColliderPtr GameObject::addPolygonCollider(const std::vector<Vec2f>& _vertices, bool _isGhost) {
        Ptr::PolygonColliderPtr _polygonCollider = std::make_shared<PolygonCollider>(this, this->transform.position, _vertices, _isGhost);
        this->components.emplace_back(_polygonCollider);
        return _polygonCollider;
    }

    Ptr::PhysicsBodyPtr GameObject::addPhysicsBody() {
        Ptr::PhysicsBodyPtr _physicsBody = std::make_shared<PhysicsBody>(this, this->transform.position);
        this->components.emplace_back(_physicsBody);
        return _physicsBody;
    }

    Ptr::SpritePtr GameObject::addSprite(const std::shared_ptr<TextureRegion>& _texture) {
        Ptr::SpritePtr _sprite = std::make_shared<Sprite>(this, _texture);
        this->components.emplace_back(_sprite);
        return _sprite;
    }

    Ptr::ComponentPtr GameObject::removeComponent(const Ptr::ComponentPtr& _component) {
        LOG_CRITICAL_CORE("REMOVE FROM GAME_OBJECT IS NOT FUNCTIONAL YET");
        return engine::Ptr::ComponentPtr();
    }
}