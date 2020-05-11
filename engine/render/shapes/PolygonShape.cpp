#include "pch.h"
#include "PolygonShape.h"

namespace engine {

    PolygonShape::PolygonShape(const std::vector<Vec2f>& _vertices, const Vec2f& _position, const Color& _color)
    : Shape(_vertices, _position, ShapeType::CONVEX_POLYGON) {  }
}