#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <engine/event/Event.h>

namespace engine {

    /// Method that will be invoked when polling the events.
    using EventCallbackFn = std::function<void (Event&)>;
    
    class Window;
    typedef std::unique_ptr<Window> WindowPtr;

    /// Options of the window that can be customize. It works as a flag system, like ImGui.
    typedef int WindowOptions;
    enum WindowOptions_ : unsigned {    WindowOptions_Minimize    = 1u << 0u, /// 00000001
                                        WindowOptions_Maximize    = 1u << 1u, /// 00000010
                                        WindowOptions_Resize      = 1u << 2u, /// 00000100
                                        WindowOptions_Visible     = 1u << 3u, /// 00001000
                                        WindowOptions_H_Scroll    = 1u << 4u, /// 00010000
                                        WindowOptions_V_Scroll    = 1u << 5u, /// 00100000
                                        WindowOptions_Disabled    = 1u << 6u  /// 01000000
    };

    /// Group of basic data of the window.
    struct WindowProperties {
        std::string title;
        unsigned int width;
        unsigned int height;

        Vec2i position;

        explicit WindowProperties(std::string _title = "Engine: ", unsigned int _width = 800, unsigned int _height = 480) :
                title(std::move(_title)), width(_width), height(_height) {  }
    };

    /// This class represents the window of the application and has methods to modify its components and get its data.
    class Window {

        public:

            virtual ~Window()                                                       = default;

            /// This method is used to update the window, poll the events and
            /// swap the buffers.
            virtual void update()                                                   = 0;

            /// Returns the width of the window.
            /// @return width of the window.
            [[nodiscard]] virtual int getWidth() const                              = 0;

            /// Returns the height of the window.
            /// @return height of the window.
            [[nodiscard]] virtual int getHeight() const                             = 0;

            /// Returns the title of the window.
            /// @return the title of the window as a string.
            virtual std::string& getTitle()                                         = 0;

            /// Sets the new title of the window.
            /// @param _title Title of the window.
            virtual void setTitle(const std::string& _title)                        = 0;

            /// Sets the new size of the window.
            /// @param _width new width of the window.
            /// @param _height new height of the window.
            virtual void setWindowSize(int _width, int _height)                     = 0;

            /// Returns the size of the window.
            /// @return Vec2i with the [width,height] of the window.
            [[nodiscard]] virtual Vec2f getWindowSize() const                       = 0;

            /// Enables or disables fullscreen mode.
            /// @param _fullscreen true or false to enable/disable.
            virtual void setFullscreen(bool _fullscreen)                            = 0;

            /// Returns if screen is fullscreen or not.
            /// @return true if is fullscreen, false otherwise.
            [[nodiscard]] virtual bool isFullscreen() const                         = 0;

            /// Sets the method that will be executed when a polled event triggers.
            /// @param _callback a void method with an Event& as parameter.
            virtual void setEventCallback(const EventCallbackFn& _callback)         = 0;

            /// Enables or disables VSync.
            /// @param _enable true/false to enable/disable.
            virtual void setVSync(bool _enable)                                     = 0;

            /// Returns if the VSync is enabled.
            /// @return true if is enabled, otherwise false.
            [[nodiscard]] virtual bool isVSyncActive() const                        = 0;

            /// Returns a raw pointer to one of the Window subclasses.
            /// @return raw pointer to WindowsWindow, LinuxWindow or MacWindow.
            [[nodiscard]] virtual void* getNativeWindow() const                     = 0;

            /// Returns the position of the window.
            /// @return Vec2i with the [x,y] of the window.
            [[nodiscard]] virtual Vec2i getPosition() const                         = 0;

            /// Sets the new position of the window.
            /// @param _position Vec2i with the new [x,y].
            virtual void setPosition(const Vec2i& _position)                        = 0;

            /// Sets the icon of the window.
            /// @param _path path to the icon, it can be png, jpg, jpeg, ico...
            virtual void setIcon(const char* _path)                                 = 0;

            /// Sets the window options.
            /// @param _op options of the window, such as resizing, maximize/minimize button...
            /// @param _allow true/false to enable/disable.
            virtual void setWindowOptions(WindowOptions _op, bool _allow)           = 0;

            static std::unique_ptr<Window> createWindow(const WindowProperties& _windowProperties = WindowProperties());
    };

}

#endif // WINDOW_H
