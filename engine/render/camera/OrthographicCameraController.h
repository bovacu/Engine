#pragma once

#ifndef ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include <engine/render/camera/OrthographicCamera.h>
#include <engine/util/Timestep.h>

#include <engine/event/WindowEvent.h>
#include <engine/event/MouseEvent.h>

namespace engine {

    class OrthographicCameraController;
    typedef std::shared_ptr<OrthographicCameraController> OrthographicCameraControllerPtr;

    class OrthographicCameraController {
        private:
            float aspectRatio;
            float zoomLevel = 2.1f;
            OrthographicCamera camera;

            bool rotation;

            Vec2f cameraPosition = { 0.0f, 0.0f};
            float cameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
            float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;

        public:
            explicit OrthographicCameraController(float _aspectRatio = 1, bool _rotation = false);

            void onUpdate(Timestep _ts);
            void onEvent(Event& _e);

            OrthographicCamera& getCamera() { return this->camera; }
            [[nodiscard]] const OrthographicCamera& getCamera() const { return this->camera; }

            [[nodiscard]] float getZoomLevel() const { return this->zoomLevel; }
            void setZoomLevel(float _level) { this->zoomLevel = _level; }

            [[nodiscard]] float getAspectRatio() const { return this->aspectRatio; }
            void setAspectRatio(float _aspectRatio) { this->aspectRatio = _aspectRatio; }

            void setCameraPosition(const Vec2f& _position) {  this->cameraPosition = _position; }
            [[nodiscard]] Vec2f getCameraPosition() const { return this->cameraPosition; }

        private:
            bool onMouseScrolled(MouseScrolledEvent& e);
            bool onWindowResized(WindowResizedEvent& e);
    };

}

#endif //ORTHOGRAPHIC_CAMERA_CONTROLLER_H
