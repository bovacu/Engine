#pragma once

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <engine/render/elements/TextureRegion.h>
#include <engine/components/Component.h>

namespace engine {

    class Sprite : public Component {
        private:
            std::shared_ptr<TextureRegion> texture;
            Vec2f position;
            bool visible;
            float rotation;
            Size scale = {1, 1};

        public:
            explicit Sprite(GameObject* _gameObject, const std::shared_ptr<TextureRegion>& _texture = nullptr);

            void setVisible(bool _visible);
            bool isVisible() const { return this->visible; }

            std::shared_ptr<TextureRegion> getTexture() { return this->texture; }
            void setTexture(const std::shared_ptr<TextureRegion>& _texture);

            float getRotation() const { return this->rotation; }
            void setRotation(float _rotation) { this->rotation = _rotation; }

            Vec2f getPosition() const { return this->position; }

            Size getScale() const { return this->scale; }
            void setScale(const Size& _scale) { this->scale = _scale; }

            void update(float _dt) override;
    };

    namespace Ptr {
        typedef std::shared_ptr<Sprite> SpritePtr;
    }
}

#endif //ENGINE_SPRITE_H
