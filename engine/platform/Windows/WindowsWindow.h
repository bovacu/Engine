#pragma once

#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include <GLFW/glfw3.h>

#include <engine/render/elements/GraphicsContext.h>
#include <engine/render/Window.h>

namespace engine {

    class WindowsWindow : public Window {
        private:
            GLFWwindow* window;
            GLFWmonitor* monitor;
            Vec2f windowPosition;
            GraphicsContextPtr context;

            struct WindowData {
                std::string title;
                int width, height;
                bool vSync;

                EventCallbackFn eventCallback;
            };
            WindowData data;

        public:
            explicit WindowsWindow(const WindowProperties& _props);
            ~WindowsWindow()                                        override;

            void update() override;

            [[nodiscard]] int getWidth() const                      override { return this->data.width; }
            [[nodiscard]] int getHeight() const                     override { return this->data.height; }

            std::string& getTitle()                                 override { return data.title; };
            void setTitle(const std::string& _title)                override { this->data.title = _title; };

            void setWindowSize(int _width, int _height)             override;
            [[nodiscard]] Vec2f getWindowSize() const               override { return  Vec2f((float)this->getWidth(), (float)this->getHeight()); }

            void setFullscreen(bool _fullscreen)                    override;
            [[nodiscard]] bool isFullscreen() const                 override;

            void setEventCallback(const EventCallbackFn& _callback) override { this->data.eventCallback = _callback; }

            void setVSync(bool enabled)                             override;
            [[nodiscard]] bool isVSyncActive() const                override;

            [[nodiscard]] void* getNativeWindow() const             override { return this->window; }

        private:
            virtual void init(const WindowProperties& _props);
            virtual void shutdown();
    };

}

#endif // WINDOW_SWINDOW_H
