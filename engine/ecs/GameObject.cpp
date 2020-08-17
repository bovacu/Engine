#include "pch.h"
#include "GameObject.h"
#include <engine/ecs/Components.h>

namespace engine {

    GameObject GameObject::newGameObject(const Vec2f& _position) {
        auto _gameObject = this->scene->createGameObject();
        auto& _transform =_gameObject.getComponent<Transform>();
        _transform.setPosition2D(_position);
        return _gameObject;
    }

}

