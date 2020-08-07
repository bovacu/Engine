#pragma once

#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

namespace engine {

    class Shape {
        public:
            virtual std::vector<Vec2f>& getVertices()   = 0;
            virtual std::vector<float>& getAngles()     = 0;
    };

    class Polygon : public Shape {
        private:
            std::vector<Vec2f> vertices;
            std::vector<float> angles;

        public:
            explicit Polygon(const std::vector<Vec2f>& _vertices);

            std::vector<Vec2f>& getVertices()  override { return this->vertices; }
            std::vector<float>& getAngles()    override { return this->angles; }
    };

    class Circle : public Shape {
        private:
            std::vector<Vec2f> vertices;
            std::vector<float> angles;
            float radius;
            Vec2f center;

        public:
            Circle(const Vec2f& _center, float _radius, int _precision = 50);

            std::vector<Vec2f>& getVertices()  override { return this->vertices; }
            std::vector<float>& getAngles()    override { return this->angles; }
    };

}

#endif //ENGINE_SHAPE_H
