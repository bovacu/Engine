#pragma once

#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "engine/render/camera/OrthographicCamera.h"
#include "engine/util/Timestep.h"

#include "engine/event/WindowEvent.h"
#include "engine/event/MouseEvent.h"

namespace engine {

    class OrthographicCameraController {
        private:
            float aspectRatio;
            float zoomLevel = 2.1f;
            OrthographicCamera camera;

            bool rotation;

            glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
            float cameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
            float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;

        public:
            OrthographicCameraController(float _aspectRatio, bool _rotation = false);

            void onUpdate(Timestep _ts);
            void onEvent(Event& _e);

            OrthographicCamera& getCamera() { return this->camera; }
            const OrthographicCamera& getCamera() const { return this->camera; }

            float getZoomLevel() const { return this->zoomLevel; }
            void setZoomLevel(float _level) { this->zoomLevel = _level; }

        private:
            bool onMouseScrolled(MouseScrolledEvent& e);
            bool onWindowResized(WindowResizedEvent& e);
    };

}

#endif //ORTHOGRAPHICCAMERACONTROLLER_H
