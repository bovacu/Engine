#pragma once

#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include <glm/glm.hpp>

namespace engine {

    class OrthographicCamera {

        private:
            glm::mat4 projectionMatrix;
            glm::mat4 viewMatrix;
            glm::mat4 viewProjectionMatrix;

            glm::vec3 position = { 0.0f, 0.0f, 0.0f };
            float rotation = 0.0f;

        public:
            OrthographicCamera(float _left, float _right, float _bottom, float _top);

            void setProjection(float _left, float _right, float _bottom, float _top);

            const glm::vec3& getPosition() const { return this->position; }
            void setPosition(const glm::vec3& _position) { this->position = _position; this->recalculateViewMatrix(); }

            float getRotation() const { return this->rotation; }
            void setRotation(float _rotation) { this->rotation = _rotation; this->recalculateViewMatrix(); }

            const glm::mat4& getProjectionMatrix() const { return this->projectionMatrix; }
            const glm::mat4& getViewMatrix() const { return this->viewMatrix; }
            const glm::mat4& getViewProjectionMatrix() const { return this->viewProjectionMatrix; }
        private:
            void recalculateViewMatrix();
    };

}




#endif // ORTHOGRAPHIC_CAMERA_H
