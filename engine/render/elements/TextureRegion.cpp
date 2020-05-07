#include "pch.h"
#include "TextureRegion.h"

#include <utility>

namespace engine {

    TextureRegion::TextureRegion(const std::shared_ptr<Texture2D>& _texture, const Vec2f &_bottomLeft, const Vec2f &_topRight)
    : texture(_texture) {
        this->textureCoords[0] = { _bottomLeft.x, _bottomLeft.y };
        this->textureCoords[1] = { _topRight.x  , _bottomLeft.y };
        this->textureCoords[2] = { _topRight.x  , _topRight.y   };
        this->textureCoords[3] = { _bottomLeft.x, _topRight.y   };

    }

    std::shared_ptr<TextureRegion> TextureRegion::createTextureRegion(const std::shared_ptr<Texture2D> &_texture, const Vec2f& _coordinates, const Size &_regionSize, const Vec2f &_numberOfRegions) {
        Vec2f _bottomLeft = { (_coordinates.x * _regionSize.width) / _texture->getWidth(), (_coordinates.y * _regionSize.height) / _texture->getHeight() };
        Vec2f _topRight   = { ((_coordinates.x + _numberOfRegions.x) * _regionSize.width) / _texture->getWidth(),
                              ((_coordinates.y + _numberOfRegions.y) * _regionSize.height) / _texture->getHeight() };

        return std::make_shared<TextureRegion>(_texture, _bottomLeft, _topRight);
    }
}