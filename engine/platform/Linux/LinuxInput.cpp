#include "pch.h"
#include <engine/input/Input.h>

#include <GLFW/glfw3.h>
#include <engine/main/Application.h>

namespace engine {

#if defined(ENGINE_PLATFORM_LINUX)
    bool Input::isKeyJustPressed(KeyCode _keyCode) {
        if(!Input::pressedKeys[_keyCode]) {
            auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
            auto _state = glfwGetKey(_window, static_cast<int32_t>(_keyCode));
            bool _result = _state == GLFW_PRESS;

            if(_result)
                Input::pressedKeys[_keyCode] = true;

            return _result;
        }

        return false;
    }

    bool Input::isKeyPressed(KeyCode _key) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_PRESS;
    }

    bool Input::isKeyReleased(KeyCode _key) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_RELEASE;
    }

    bool Input::isMouseJustPressed(MouseCode _mouseButton) {
        if(!Input::pressedMouseButtons[_mouseButton]) {
            auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
            auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_mouseButton));
            bool _result = _state == GLFW_PRESS;

            if(_result)
                Input::pressedMouseButtons[_mouseButton] = true;

            return _result;
        }

        return false;
    }

    bool Input::isMousePressed(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_PRESS;
    }

    bool Input::isMouseReleased(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_RELEASE;
    }

    Vec2i Input::getMousePosition() {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int _height, _width;
        glfwGetWindowSize(_window, &_width, &_height);

        double _xPos, _yPos;
        glfwGetCursorPos(_window, &_xPos, &_yPos);
        return { (int)_xPos, (int)(_height - _yPos) };
    }

    int Input::getMouseX() {
        return getMousePosition().x;
    }

    int Input::getMouseY() {
        return getMousePosition().y;
    }
#endif

}