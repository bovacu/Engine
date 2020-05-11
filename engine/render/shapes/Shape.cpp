#include "pch.h"
#include "Shape.h"

namespace engine {

    Shape::Shape(const std::vector<Vec2f>& _vertices, const Vec2f& _position, const ShapeType& _shapeType) : vertices(_vertices), shapeType(_shapeType) {
        this->size = this->calSize(_vertices);
        this->position = _position;
    }

    Size Shape::calSize(const std::vector<Vec2f>& _vertices) {
        ENGINE_CORE_ASSERT(_vertices.size(), "Can't draw shape without vertices");
        float _minY = _vertices[0].y, _maxY = _minY;
        float _minX = _vertices[0].x, _maxX = _minX;

        for(auto& _vertex : _vertices) {
            _minX = std::min(_vertex.x, _minX);
            _maxX = std::max(_vertex.x, _maxX);

            _minY = std::min(_vertex.y, _minY);
            _maxY = std::max(_vertex.y, _maxY);
        }

        return Size( std::abs(_maxX - _minX) , std::abs(_minY - _maxY));
    }
}