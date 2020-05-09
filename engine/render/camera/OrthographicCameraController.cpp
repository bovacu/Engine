#include "pch.h"
#include "OrthographicCameraController.h"

#include <engine/input/Input.h>
#include <engine/main/Application.h>

namespace engine {

    OrthographicCameraController::OrthographicCameraController(float _aspectRatio, bool _rotation)
            : aspectRatio(_aspectRatio),
            camera(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel),
            rotation(_rotation) {
        // Auto updates the aspect ratio directly to the screen size, this way, there's no need to pass it as an argument.
        auto& _app = Application::get();
        this->aspectRatio = _app.getWindowSize().x / _app.getWindowSize().y;
        this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel);
    }

    void OrthographicCameraController::onUpdate(Timestep _ts) {
        if (Input::isKeyPressed(KEY_A)) {
            this->cameraPosition.x -= cos(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
            this->cameraPosition.y -= sin(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
        }
        else if (Input::isKeyPressed(KEY_D)) {
            this->cameraPosition.x += cos(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
            this->cameraPosition.y += sin(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
        }

        if (Input::isKeyPressed(KEY_W)) {
            this->cameraPosition.x += -sin(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
            this->cameraPosition.y += cos(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
        }
        else if (Input::isKeyPressed(KEY_S)) {
            this->cameraPosition.x -= -sin(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
            this->cameraPosition.y -= cos(glm::radians(this->cameraRotation)) * this->cameraTranslationSpeed * _ts;
        }

        if (this->rotation) {
            if (Input::isKeyPressed(KEY_Q))
                this->cameraRotation += this->cameraRotationSpeed * _ts;
            if (Input::isKeyPressed(KEY_E))
                this->cameraRotation -= this->cameraRotationSpeed * _ts;

            if (this->cameraRotation > 180.0f)
                this->cameraRotation -= 360.0f;
            else if (this->cameraRotation <= -180.0f)
                this->cameraRotation += 360.0f;

            this->camera.setRotation(this->cameraRotation);
        }

        this->camera.setPosition({this->cameraPosition.x, this->cameraPosition.y, 0.0f});

        this->cameraTranslationSpeed = this->zoomLevel;
    }

    void OrthographicCameraController::onEvent(Event& _e) {

        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatchEvent<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& _e) {
        this->zoomLevel -= _e.getScrollY() * 0.25f;
        this->zoomLevel = std::max(this->zoomLevel, 0.25f);
        this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel);
        return false;
    }

    bool OrthographicCameraController::onWindowResized(WindowResizedEvent& _e) {
        this->aspectRatio = (float)_e.getWidth() / (float)_e.getHeight();
        this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel);
        return false;
    }

}