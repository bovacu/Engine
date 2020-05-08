#include "pch.h"
#include "Sprite.h"
#include <engine/gameObject/GameObject.h>

namespace engine {

    Sprite* Sprite::create(GameObject* _gameObject, const std::shared_ptr<TextureRegion>& _texture) {
        return std::make_shared<Sprite>(_gameObject, _texture).get();
    }

    /// ----------------------------------------------------------------------------------------------------------------

    Sprite::Sprite(GameObject *_gameObject, const std::shared_ptr<TextureRegion>& _texture) : Component(_gameObject, ComponentType::SPRITE)
    , texture(_texture) {
        this->position = _gameObject->transform.position;
        this->rotation = _gameObject->transform.rotation;
    }

    void Sprite::update(float _dt) {
        this->position = this->gameObject->transform.position;
        this->rotation = this->gameObject->transform.rotation;
    }

    void Sprite::setVisible(bool _visible) {
        this->visible = _visible;
    }

    void Sprite::setTexture(const std::shared_ptr<TextureRegion>& _texture) {
        this->texture = _texture;
    }


}