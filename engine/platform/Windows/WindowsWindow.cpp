#include "pch.h"
#include "WindowsWindow.h"

#include "engine/input/Input.h"

#include "engine/event/WindowEvent.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"

#include "engine/render/Renderer.h"

namespace engine {

    static uint8_t GLFWWindowCount = 0;

    static void GLFWErrorCallback(int _error, const char* _description) {
         LOG_ERROR_CORE("GLFW Error ({0}): {1}", _error, _description);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& _props) : window(nullptr) {
        this->init(_props);
    }

    WindowsWindow::~WindowsWindow() {
        this->shutdown();
    }

    void WindowsWindow::init(const WindowProperties& _props) {

        this->data.title = _props.title;
        this->data.width = _props.width;
        this->data.height = _props.height;

        #ifdef ENGINE_DEBUG
            LOG_WARN_CORE("Running in debug mode");
            LOG_INFO_CORE("Creating window {0} ({1}, {2})", _props.title, _props.width, _props.height);
        #endif

        if (GLFWWindowCount == 0) {
            int success = glfwInit();
            ENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            #if defined(ENGINE_DEBUG)
            if (Renderer::getAPI() == RenderAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            #endif
            this->window = glfwCreateWindow((int)_props.width, (int)_props.height, this->data.title.c_str(), nullptr, nullptr);
            ++GLFWWindowCount;
        }

        this->context = GraphicsContext::create(this->window);
        this->context->init();

        glfwSetWindowUserPointer(this->window, &this->data);
        this->setVSync(false);

        this->monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* _mode = glfwGetVideoMode(this->monitor);
        if (!_mode)
            return;

        int _monitorX, _monitorY;
        glfwGetMonitorPos(this->monitor, &_monitorX, &_monitorY);
        int _windowWidth, _windowHeight;
        glfwGetWindowSize(this->window, &_windowWidth, &_windowHeight);

        glfwSetWindowPos(this->window, _monitorX + (_mode->width - _windowWidth) / 2, _monitorY + (_mode->height - _windowHeight) / 2);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(this->window, [](GLFWwindow* _window, int _width, int _height) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);
            _data.width = _width;
            _data.height = _height;

            WindowResizedEvent _event(_width, _height);
            _data.eventCallback(_event);
        });

        glfwSetWindowCloseCallback(this->window, [](GLFWwindow* _window) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);
            WindowClosedEvent _event;
            _data.eventCallback(_event);
        });

        glfwSetKeyCallback(this->window, [](GLFWwindow* _window, int _key, int _scanCode, int _action, int _mods) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);

            switch (_action) {
                case GLFW_PRESS: {
                    KeyPressedEvent _event(static_cast<KeyCode>(_key), 0);
                    _data.eventCallback(_event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent _event(static_cast<KeyCode>(_key));
                    _data.eventCallback(_event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent _event(static_cast<KeyCode>(_key), 1);
                    _data.eventCallback(_event);
                    break;
                }
            }
        });

        glfwSetCharCallback(this->window, [](GLFWwindow* _window, unsigned int _key) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);

            KeyTypedEvent _event(static_cast<KeyCode>(_key));
            _data.eventCallback(_event);
        });

        glfwSetMouseButtonCallback(this->window, [](GLFWwindow* _window, int _button, int _action, int _mods) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);

            switch (_action) {
                case GLFW_PRESS : {
                    MouseButtonPressedEvent _event(static_cast<MouseCode>(_button));
                    _data.eventCallback(_event);
                    break;
                }
                case GLFW_RELEASE : {
                    MouseButtonReleasedEvent _event(static_cast<MouseCode>(_button));
                    _data.eventCallback(_event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(this->window, [](GLFWwindow* _window, double _xOffset, double _yOffset) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);

            MouseScrolledEvent _event((float)_xOffset, (float)_yOffset);
            _data.eventCallback(_event);
        });

        glfwSetCursorPosCallback(this->window, [](GLFWwindow* _window, double _xPos, double _yPos) {
            WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(_window);

            MouseMovedEvent _event((float)_xPos, (float)_yPos);
            _data.eventCallback(_event);
        });
    }

    void WindowsWindow::setWindowSize(int _width, int _height) {
        this->data.width = _width;
        this->data.height = _height;
        glfwSetWindowSize(this->window, _width, _height);
    }

    void WindowsWindow::shutdown() {
        glfwDestroyWindow(this->window);
        --GLFWWindowCount;

        if (GLFWWindowCount == 0) {
            glfwTerminate();
        }
    }

    void WindowsWindow::update() {
        glfwPollEvents();
        this->context->swapBuffers();
    }

    void WindowsWindow::setVSync(bool _enabled) {
        if (_enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        this->data.vSync = _enabled;
    }

    bool WindowsWindow::isVSyncActive() const {
        return this->data.vSync;
    }

    void WindowsWindow::setFullscreen(bool _fullscreen) {
        if ( _fullscreen ) {
            // backup window position and window size
            int _x, _y;
            glfwGetWindowPos( this->window, &_x, &_y );
            this->windowPosition = {(float)_x, (float)_y};
            glfwGetWindowSize( this->window, &this->data.width, &this->data.height );

            // get resolution of monitor
            const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            // switch to full screen
            glfwSetWindowMonitor( this->window, this->monitor, 0, 0, mode->width, mode->height, 0 );
            this->setWindowSize(mode->width, mode->height);
        }
        else {
            // restore last window size and position
            glfwSetWindowMonitor( this->window, nullptr,  (int)this->windowPosition.x, (int)this->windowPosition.y
                    , this->data.width, this->data.height, 0 );
            this->setWindowSize(this->data.width, this->data.height);
        }
    }

    bool WindowsWindow::isFullscreen() const {
        LOG_CRITICAL_CORE("isFullscreen from WindowsWindow not implemented yet");
        return false;
    }

}