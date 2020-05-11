#pragma once

#ifndef ENGINE_POLYGON_SHAPE_H
#define ENGINE_POLYGON_SHAPE_H

#include <engine/render/shapes/Shape.h>

namespace engine {

    class PolygonShape : public Shape {
        public:
            explicit PolygonShape(const std::vector<Vec2f>& _vertices, const Vec2f& _position, const Color& _color = {0.0f, 1.0f, 0.0f, 0.25f});
    };

}

#endif //ENGINE_POLYGON_SHAPE_H
