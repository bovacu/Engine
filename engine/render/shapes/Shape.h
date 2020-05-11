#pragma once

#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

namespace engine {

    enum ShapeType { NONE_SHAPE, RECTANGLE, CONVEX_POLYGON, CIRCLE, NON_CONVEX_POLYGON };

    class Shape {
        private:
            std::vector<Vec2f> vertices;
            Size size;
            Color color;
            ShapeType shapeType;
            Vec2f position;

        public:
            explicit Shape(const std::vector<Vec2f>& _vertices, const Vec2f& _position = {0, 0}, const ShapeType& _shapeType = ShapeType::NONE_SHAPE);

            [[nodiscard]] const std::vector<Vec2f>& getVertices() const { return this->vertices; }
            [[nodiscard]] Size getSize() const { return this->size; }
            [[nodiscard]] ShapeType getShapeType() const { return this->shapeType; }

            [[nodiscard]] Color getColor() const { return this->color; }
            void setColor(const Color& _color) { this->color = _color; }

            [[nodiscard]] Vec2f getPosition() const { return this->position; }
            void setPosition(const Vec2f& _position) { this->position = _position; }

        private:
            Size calSize(const std::vector<Vec2f>& _vertices);

    };

}

#endif //ENGINE_SHAPE_H
