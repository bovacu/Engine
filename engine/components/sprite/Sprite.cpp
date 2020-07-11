#include "pch.h"
#include "Sprite.h"
#include <engine/gameObject/GameObject.h>
#include <engine/main/Application.h>

namespace engine {

    SpritePtr Sprite::create(GameObject* _gameObject, const TextureRegionPtr& _texture) {
        return std::make_shared<Sprite>(_gameObject, _texture);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    Sprite::Sprite(GameObject* _gameObject, const TextureRegionPtr& _texture) : Component(_gameObject, ComponentType::SPRITE)
    , texture(_texture) {
        this->position = _gameObject->transform.position;
        this->rotation = _gameObject->transform.rotation;
        this->size = { _texture->getSize().width, _texture->getSize().height };
    }

    void Sprite::update(float _dt) {
        this->position = this->gameObject->transform.position;
        this->rotation = this->gameObject->transform.rotation;
    }

    void Sprite::setVisible(bool _visible) {
        this->visible = _visible;
    }

    void Sprite::setTexture(const TextureRegionPtr& _texture) {
        this->texture = _texture;
    }


}