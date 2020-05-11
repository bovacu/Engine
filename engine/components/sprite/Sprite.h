#pragma once

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <engine/render/elements/TextureRegion.h>
#include <engine/components/Component.h>

namespace engine {

    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;

    class Sprite : public Component {
        private:
            TextureRegionPtr texture;
            Vec2f position;
            bool visible;
            float rotation;
            Size size = {1, 1};
            Size scale = {1, 1};

        public:
            explicit Sprite(GameObject* _gameObject, const TextureRegionPtr& _texture = nullptr);

            static SpritePtr create(GameObject* _gameObject, const TextureRegionPtr& _texture = nullptr);

            void setVisible(bool _visible);
            [[nodiscard]] bool isVisible() const { return this->visible; }

            TextureRegionPtr getTexture() { return this->texture; }
            void setTexture(const TextureRegionPtr& _texture);

            [[nodiscard]] float getRotation() const { return this->rotation; }
            void setRotation(float _rotation) { this->rotation = _rotation; }

            [[nodiscard]] Vec2f getPosition() const { return this->position; }
            void setPosition(const Vec2f& _position) { this->position = _position; }

            [[nodiscard]] Size getScale() const { return this->scale; }
            void setScale(const Size& _scale) { this->scale = _scale; }

            [[nodiscard]] Size getSize() const { return this->size; }

            void update(float _dt) override;
    };
}

#endif //ENGINE_SPRITE_H
