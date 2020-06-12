#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <engine/event/Event.h>

namespace engine {

    using EventCallbackFn = std::function<void (Event&)>;
    
    class Window;
    typedef std::unique_ptr<Window> WindowPtr;

    struct WindowProperties {
        std::string title;
        unsigned int width;
        unsigned int height;

        Vec2i position;

        explicit WindowProperties(std::string _title = "Engine: ", unsigned int _width = 800, unsigned int _height = 480) :
                title(std::move(_title)), width(_width), height(_height) {  }
    };

    class Window {
        public:

            virtual ~Window()                                               = default;

            virtual void update()                                           = 0;

            [[nodiscard]] virtual int getWidth() const                      = 0;
            [[nodiscard]] virtual int getHeight() const                     = 0;

            virtual std::string& getTitle()                                 = 0;
            virtual void setTitle(const std::string& _title)                = 0;

            virtual void setWindowSize(int _width, int _height)             = 0;
            virtual Vec2f getWindowSize() const                             = 0;

            virtual void setFullscreen(bool _fullscreen)                    = 0;
            [[nodiscard]] virtual bool isFullscreen() const                 = 0;

            virtual void setEventCallback(const EventCallbackFn& _callback) = 0;
            virtual void setVSync(bool _enable)                             = 0;
            [[nodiscard]] virtual bool isVSyncActive() const                = 0;

            [[nodiscard]] virtual void* getNativeWindow() const             = 0;

            [[nodiscard]] virtual Vec2i getPosition() const                 = 0;
            virtual void setPosition(const Vec2i& _position)                = 0;

            static std::unique_ptr<Window> createWindow(const WindowProperties& _windowProperties = WindowProperties());
    };

}

#endif // WINDOW_H
