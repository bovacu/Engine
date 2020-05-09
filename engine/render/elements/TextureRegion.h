#pragma once

#ifndef ENGINE_TEXTURE_REGION_H
#define ENGINE_TEXTURE_REGION_H

#include <engine/render/elements/Texture.h>

namespace engine {

    class TextureRegion;
    typedef std::shared_ptr<TextureRegion> TextureRegionPtr;

    class TextureRegion {
        private:
            std::shared_ptr<Texture2D> texture;
            Vec2f textureCoords[4];

        public:
            TextureRegion(const Texture2DPtr& _texture, const Vec2f& _bottomLeft, const Vec2f& _topRight);
            Texture2DPtr getTexture() { return this->texture; }
            const Vec2f* getTextureCoords() { return this->textureCoords; }

            static TextureRegionPtr create(const Texture2DPtr& _texture, const Vec2f& _coordinates, const Size& _regionSize, const Vec2f& _numberOfRegions = {1, 1});
    };

}

#endif //ENGINE_TEXTURE_REGION_H
