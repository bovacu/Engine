#include "pch.h"
#include "LinuxInput.h"

#include <GLFW/glfw3.h>
#include <engine/main/Application.h>

namespace engine {

    bool LinuxInput::isKeyJustPressed_v(KeyCode _keyCode) {
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

    bool LinuxInput::isKeyPressed_v(KeyCode _key) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_PRESS;
    }

    bool LinuxInput::isKeyReleased_v(KeyCode _key) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_RELEASE;
    }

    bool LinuxInput::isMousePressed_v(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_PRESS;
    }

    bool LinuxInput::isMouseReleased_v(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_RELEASE;
    }

    Vec2f LinuxInput::getMousePosition_v() {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int _height, _width;
        glfwGetWindowSize(_window, &_width, &_height);

        double _xPos, _yPos;
        glfwGetCursorPos(_window, &_xPos, &_yPos);
        return { (float)_xPos, (float)(_height - _yPos) };
    }

    float LinuxInput::getMouseX_v() {
        return getMousePosition_v().x;
    }

    float LinuxInput::getMouseY_v() {
        return getMousePosition_v().y;
    }

}