#include "pch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>
#include "engine/main/Application.h"

namespace engine {

    bool WindowsInput::isKeyPressed_v(KeyCode _key) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return state == GLFW_PRESS;
    }

    bool WindowsInput::isKeyDown_v(KeyCode _key) {
        LOG_WARN_CORE("IS KEY DOWN IS NOT TESTED TO BE WORKING OKAY");
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_REPEAT;
    }

    bool WindowsInput::isKeyReleased_v(KeyCode _key) {
        LOG_WARN_CORE("IS KEY RELEASED IS NOT TESTED TO BE WORKING OKAY");
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetKey(_window, static_cast<int32_t>(_key));
        return _state == GLFW_RELEASE;
    }

    bool WindowsInput::isMousePressed_v(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_PRESS;
    }

    bool WindowsInput::isMouseDown_v(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_REPEAT;
    }

    bool WindowsInput::isMouseReleased_v(MouseCode _button) {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto _state = glfwGetMouseButton(_window, static_cast<int32_t>(_button));
        return _state == GLFW_RELEASE;
    }

    Vec2f WindowsInput::getMousePosition_v() {
        auto _window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double _xPos, _yPos;
        glfwGetCursorPos(_window, &_xPos, &_yPos);
        return { (float)_xPos, (float)_yPos };
    }

    float WindowsInput::getMouseX_v() {
        return getMousePosition_v().x;
    }

    float WindowsInput::getMouseY_v() {
        return getMousePosition_v().y;
    }

}