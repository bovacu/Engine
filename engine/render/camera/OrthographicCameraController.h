#pragma once

#ifndef ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include <engine/render/camera/OrthographicCamera.h>
#include <engine/util/Delta.h>

#include <engine/event/WindowEvent.h>
#include <engine/event/MouseEvent.h>

namespace engine {

    class OrthographicCameraController;
    typedef std::shared_ptr<OrthographicCameraController> OrthographicCameraControllerPtr;

    class OrthographicCameraController {
        private:
            float aspectRatio;
            float zoomLevel = 1.f;
            OrthographicCamera camera;

            bool rotation;

            Vec2f cameraPosition = { 0.0f, 0.0f};
            float cameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
            float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;

        public:
            explicit OrthographicCameraController(bool _aspectRatio = true, bool _rotation = false);
            explicit OrthographicCameraController(float _aspectRatio, bool _rotation = false);

            void onUpdate(Delta _ts);
            void onEvent(Event& _e);

            void onResize(float _width, float _height);

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
