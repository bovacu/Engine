#include "pch.h"
#include "OrthographicCameraController.h"

#include <engine/main/Application.h>

namespace engine {

    OrthographicCameraController::OrthographicCameraController(bool _aspectRatio, bool _rotation)
            : aspectRatio(1),
            camera(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel),
            rotation(_rotation) {
        OrthographicCamera::usingAspectRatio = _aspectRatio;

        // Auto updates the aspect ratio directly to the screen size, this way, there's no need to pass it as an argument.
        auto& _app = Application::get();
        this->aspectRatio = (float)_app.getWindowSize().x / (float)_app.getWindowSize().y;

        if(OrthographicCamera::usingAspectRatio) {
            this->zoomLevel = 12.0f;
            this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel,
                                       -this->zoomLevel, this->zoomLevel);
        } else {
            this->zoomLevel = 0.5f;
            this->camera.setProjection(-_app.getWindowSize().x * this->zoomLevel,
                                       _app.getWindowSize().x * this->zoomLevel,
                                       -_app.getWindowSize().y * this->zoomLevel,
                                       _app.getWindowSize().y * this->zoomLevel);
        }
    }

    void OrthographicCameraController::onUpdate(Timestep _ts) {
        this->camera.setPosition({this->cameraPosition.x, this->cameraPosition.y, 0.0f});
        this->cameraTranslationSpeed = this->zoomLevel;
    }

    void OrthographicCameraController::onEvent(Event& _e) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatchEvent<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& _e) {
//        this->zoomLevel -= _e.getScrollY() * 0.05f;
//        this->zoomLevel = std::max(this->zoomLevel, 0.05f);
//
//        if(OrthographicCamera::usingAspectRatio)
//            this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel);
//        else {
//            auto& _app = Application::get();
//            this->camera.setProjection(-_app.getWindowSize().x * this->zoomLevel, _app.getWindowSize().x * this->zoomLevel, -_app.getWindowSize().y *this->zoomLevel,
//                                       _app.getWindowSize().y * this->zoomLevel);
//        }
        return false;
    }

    bool OrthographicCameraController::onWindowResized(WindowResizedEvent& _e) {
        this->aspectRatio = (float)_e.getWidth() / (float)_e.getHeight();

        if(OrthographicCamera::usingAspectRatio)
            this->camera.setProjection(-this->aspectRatio * this->zoomLevel, this->aspectRatio * this->zoomLevel, -this->zoomLevel, this->zoomLevel);
        else {
            auto& _app = Application::get();
            this->camera.setProjection(-_app.getWindowSize().x * this->zoomLevel, _app.getWindowSize().x * this->zoomLevel, -_app.getWindowSize().y *this->zoomLevel,
                                       _app.getWindowSize().y * this->zoomLevel);
        }
        return false;
    }

}