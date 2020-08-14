#include "pch.h"
#include "Application.h"

#include <engine/render/Renderer.h>
#include <GLFW/glfw3.h>

namespace engine {

    Application *Application::gameInstance = nullptr;

    Application::Application() {
        ENGINE_CORE_ASSERT(!Application::gameInstance, "Application already exists!");
        Application::gameInstance = this;
        this->window = Window::createWindow();
        this->window->setEventCallback(ENGINE_BIND_EVENT_FN(Application::onEvent));
        this->lastFrame = 0;

        Renderer::init();

        this->imGuiLayer = new ImGuiLayer();
        pushOverlay(this->imGuiLayer);

        if(this->timePerFrame < 0)
            this->timePerFrame = 1.0f / 60.f;

        this->window->setVSync(true);
    }

    Application::~Application() {
        Renderer::shutdown();
    }

    void Application::onRun() {
        float _accumulator = 0;

        while (this->running) {

            auto _time = (float) glfwGetTime();
            Delta _dt = _time - this->lastFrame;
            this->lastFrame = _time;
            _accumulator += _dt;

            if (!this->minimized) {

                while (_accumulator >= this->timePerFrame) {
                    _accumulator -= this->timePerFrame;
                    this->onFixedUpdate(this->timePerFrame);
                }

                this->onUpdate(_dt);
                this->onRender(_dt);

//                #ifdef ENGINE_DEBUG
                    this->updateFps();
//                #endif
            }

            this->window->update();
        }
    }

    void Application::onEvent(Event &_e) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<WindowClosedEvent>(ENGINE_BIND_EVENT_FN(Application::onWindowClosed));
        dispatcher.dispatchEvent<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(Application::onWindowResized));

        for (auto _it = this->layerStack.rbegin(); _it != this->layerStack.rend(); ++_it) {
            (*_it)->onEvent(_e);
            if (_e.handled)
                break;
        }
    }

    void Application::onFixedUpdate(Delta _fixedDt) {
        for (Layer* _layer : this->layerStack)
            _layer->onFixedUpdate(_fixedDt);
    }

    void Application::onUpdate(Delta _dt) {
        for (Layer* _layer : this->layerStack)
            _layer->onUpdate(_dt);
    }

    void Application::onRender(Delta _dt) {
        for (Layer* _layer : this->layerStack)
            _layer->onRender(_dt);

        this->imGuiLayer->begin();
            for (Layer* _layer : this->layerStack)
                _layer->onImGuiRender(_dt);
        this->imGuiLayer->end();
    }

    bool Application::onWindowClosed(WindowClosedEvent &_e) {
        this->running = false;
        return true;
    }

    bool Application::onWindowResized(WindowResizedEvent &_e) {
        if (_e.getWidth() == 0 || _e.getHeight() == 0) {
            this->minimized = true;
            return false;
        }

        this->minimized = false;
        Renderer::onWindowResize(_e.getWidth(), _e.getHeight());

        return false;
    }

    int Application::getFps() const { return (int)this->fpsCounter; }

    void Application::updateFps() {
        if (this->clock.getElapsedTimeSc() >= 1.f) {
            fpsCounter = frameCounter;
            frameCounter = 0;
            this->clock.restart();
        }
        ++frameCounter;
    }

    void Application::setTitle(const std::string& _title) {
        this->window->setTitle(_title);
    }

    void Application::setFullscreen(bool _fullscreen) {
        LOG_CRITICAL_CORE("Fullscreen not working properly yet, don't use it");
        this->window->setFullscreen(_fullscreen);
    }

    void Application::setVSync(bool _vsync) {
        this->window->setVSync(_vsync);
    }

    bool Application::isVSync() {
        return this->window->isVSyncActive();
    }

    void Application::setWindowSize(int _width, int _height) {
        this->window->setWindowSize(_width, _height);
        Renderer::onWindowResize(_width, _height);
    }

    void Application::pushLayer(Layer* _layer) {
        this->layerStack.pushLayer(_layer);
        _layer->onInit();
    }

    void Application::pushOverlay(Layer* _layer) {
        this->layerStack.pushOverlay(_layer);
        _layer->onInit();
    }

    void Application::popLayer(Layer* _layer) {
        this->layerStack.popLayer(_layer);
        _layer->onEnd();
    }

    void Application::popOverlay(Layer* _layer) {
        this->layerStack.popOverlay(_layer);
        _layer->onEnd();
    }

    void Application::closeApplication() {
        this->running = false;
    }

}