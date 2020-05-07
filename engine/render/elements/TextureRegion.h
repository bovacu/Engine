#pragma once

#ifndef ENGINE_TEXTUREREGION_H
#define ENGINE_TEXTUREREGION_H

#include <engine/render/elements/Texture.h>

namespace engine {

    class TextureRegion {
        private:
            std::shared_ptr<Texture2D> texture;
            Vec2f textureCoords[4];

        public:
            TextureRegion(const std::shared_ptr<Texture2D>& _texture, const Vec2f& _bottomLeft, const Vec2f& _topRight);
            const std::shared_ptr<Texture2D> getTexture() { return this->texture; }
            const Vec2f* getTextureCoords() { return this->textureCoords; }

            static std::shared_ptr<TextureRegion> createTextureRegion(const std::shared_ptr<Texture2D>& _texture, const Vec2f& _coordinates, const Size& _regionSize, const Vec2f& _numberOfRegions = {1, 1});
    };

}

#endif //ENGINE_TEXTUREREGION_H
