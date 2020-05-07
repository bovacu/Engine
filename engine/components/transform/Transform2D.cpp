#include "pch.h"
#include "Transform2D.h"

namespace engine {

    Transform2D::Transform2D() : position(0, 0), rotation(0), scale(1, 1) {  }
    Transform2D::Transform2D(const Vec2f& _position, float _rotation, const Vec2f& _scale) : position(_position), rotation(_rotation), scale(_scale) {  }

    void Transform2D::setPosition(const Vec2f& _position) {
        this->position = _position;
    }

    void Transform2D::setPosition(float _x, float _y) {
        this->position.x = _x;
        this->position.y = _y;
    }

    void Transform2D::translate(const Vec2f& _position) {
        this->position += _position;
    }

    void Transform2D::translate(float _x, float _y) {
        this->position.x += _x;
        this->position.y += _y;
    }
}