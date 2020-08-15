#pragma once

#ifndef ENGINE_SCENECAMERA_H
#define ENGINE_SCENECAMERA_H

#include <engine/render/camera/Camera.h>

namespace engine {

    class SceneCamera : public engine::Camera {
        private:
            float orthographicSize = 10.f, distanceNear = -1.f, distanceFar = 1.f, aspectRatio = 0.f;

        private:
            void recalcProjectionMatrix();

        public:
            SceneCamera();
            ~SceneCamera() override = default;

            void setOrthographic(float _size, float _distanceNear, float _distanceFar);
            void setViewportSize(int _width, int _height);

            [[nodiscard]] float getOrthographicSize() const { return this->orthographicSize; }
            [[nodiscard]] float getNear() const { return this->distanceNear; }
            [[nodiscard]] float getFar() const { return this->distanceFar; }
            void setOrthographicSize(float _orthographicSize) { this->orthographicSize = _orthographicSize; }
    };

}

#endif //ENGINE_SCENECAMERA_H
