#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include <engine/util/EngineUtil.h>
#include <engine/main/Core.h>
#include <engine/render/Window.h>
#include <engine/event/WindowEvent.h>
#include <engine/util/Timestep.h>
#include <engine/util/Clock.h>
#include <engine/render/layer/Layer.h>
#include <engine/render/layer/LayerStack.h>
#include <engine/render/layer/ImGuiLayer.h>
#include <engine/input/Input.h>


/*  =================================================================================================
    =                                           GAME                                                =
    =================================================================================================
    = (*) This class is the core of the engine, to use the engine, your application class           =
	=	  must extend this class.                                                                   =
	=                                                                                               =
    = (*) onEvent:  																				=
    = 		- This method is used to managed any window event or keyboard, mouse, gamepad event     =
    =         that isn't handle by isKeyPressed, isBMouseButtonDown...                              =
    =                                                                                               =
    = (*) onUpdate:																					=
    =		- This method is used to update the objects and also where input should be captured .   =
    =         It can also be used to update physics, but that's recommended on the fixUpdate        =
    =         as it's more precised.                                                                =
	=																								=
	= (*) onFixUpdate:																				=
	=		- This is where all calcs related to physics should be computed, as it runs a fix       =
    =         number of times per second and with a fixed delta of 0.016...                         =
    =                                                                                               =
	= (*) onRender:																					=
	=		- This methods is in charge of making things to draw on screen once everything on the	=
	=		  game has been correctly updated.														=
	=																								=
    =================================================================================================
    =                                                                                               =
    = (*) The game can be played with or without VSync.												=
    =                                                                                               =
    ================================================================================================= 
*/

/// We declare this here and later the int main function as friend so in EngineExecuter we don't need to include any
/// kind of reference, it is completely clean. Why we need to take the int main out of this class is explained in
/// EngineExecuter.h file.
int main(int argc, char** argv);

namespace engine {

	class Application {
		private:
	        float timePerFrame = -1;
			int fps;
            float lastFrame = 0.0f;
			Timestep dt;
			unsigned int fpsCounter = 0, frameCounter = 0;

	    private:
            bool running = true;
            bool minimized = false;

	    private:
			WindowPtr window;
			static Application* gameInstance;
            LayerStack layerStack;
            ImGuiLayer* imGuiLayer;
            Clock clock;

            /// This is the int main friend I mentioned before when declaring the function. The :: is used to referer
            /// the global namespace, therefore we can refer to any function.
            friend int ::main(int argc, char** argv);

            /// This method updates the FPS counter each second
            void updateFps();

        public:
	        Application();
            virtual ~Application();

            void closeApplication();

            /// This method provides the number of FPS in any given moment.
            /// @return the number of FPS
            int getFps() const;

            /// Sets the title of the game's window.
            /// @param [in] _title The title of the window
            void setTitle(const std::string& _title);

            /// Gives the title of the window.
            /// @return the title of the window.
            [[nodiscard]] const std::string& getTitle() const       { return this->window->getTitle(); }

            /// Sets the size of the window.
            /// @param _width Width of the window.
            /// @param _height Height of the window.
            void setWindowSize(int _width, int _height);

            /// Returns the size of the window as a Vec2i.
            ///@return Vec2i with the size of the window.
            [[nodiscard]] Vec2i getWindowSize() const               { return Vec2i(this->window->getWidth(), this->window->getHeight()); }

            /// Returns 1 / FPS
            /// @return The FPS as milliseconds.
            [[nodiscard]] float getTimePerFrame() const             { return this->timePerFrame; }

            /// Enables or disables the VSync
            /// @param true or false depending if you want to enable(true) or disable(false).
            void setVSync(bool _vsync);

            /// Returns if the VSync is active.
            /// @return If VSync is active.
            bool isVSync();

            /// Returns the delta (time passed between the actual frame and the previous one) on any moment.
            /// @return The current delta.
            [[nodiscard]] Timestep getDelta() const                 { return this->dt; }

            /// Returns in a Vec2i the current position of the mouse on the screen.
            /// @return Vec2i with the [x,y] coordinates.
            [[nodiscard]] Vec2i getMousePos() const                 { return Vec2i( (int)(-this->getWindowSize().x / 2.f + Input::getMouseX()),
                                                                                (int)(this->getWindowSize().y / 2.f - Input::getMouseY())); }
            /// Enables or disables the fullscreen mode.
            /// @param _fullscreen Enables(true) or disables(false) the fullscreen.
            void setFullscreen(bool _fullscreen);

            /// Returns if fullscreen is active.
            /// @return If fullscreen is active.
            [[nodiscard]] bool isFullscreen() const                 {  return this->window->isFullscreen(); }

            /// Sets the position of the window on the screen.
            /// @param _position Vec2i with [x,y] coordinates to move the window.
            void setPosition(const Vec2i& _position)                { this->window->setPosition(_position); }

            /// Returns a Vec2i with the current position of the window.
            /// @return Vec2i with the position of the window.
            [[nodiscard]] Vec2i getPosition() const                 { return this->window->getPosition(); }

            /// Allows you to set the icon of the window.
            /// @param _path Path to the image to be used as icon, can be in png, jpg, jpeg, ico...
            void setAppIcon(const char* _path)                      { this->window->setIcon(_path); }

            /// Allows to enable or disable some window features, such as:
            ///     - Minimize/Maximize button.
            ///     - Resizing.
            ///     - Enabling the window.
            ///     - Visibility of the window.
            ///     - V/H Scroll of the window.
            /// Multiple options can be concatenated by | operator, but all must be set to same value of _allow
            /// @param _op The options to set to true or false
            /// @param _allow true/false depending if disables or enables
            /// @sa WindowOptions_ in Window.h
            void setWindowOptions(WindowOptions _op, bool _allow)   { this->window->setWindowOptions(_op, _allow); }

        public:
	        /// This method is the main loop of the game, runs until the window is closed or running attribute is set
	        /// to false. Runs with an independent frame rate loop.
            void onRun();

            /// This methods handles all the events that can't be handle in any given moment, such as resizing the
            /// window, closing the window, moving it...
            /// @param The Event to be handled
            /// @sa Event
            void onEvent(Event& _event);

            /// This method handles the non-physics based updates of the game and also is used to capture all the inputs.
            /// @param _dt The amount of time passed from the previous frame to this current one.
            void onUpdate(Timestep _dt);

            /// This method is like onUpdate, but this one handles the physics-based updates.
            /// @param _fixedDt Fixed value to multiply the physics movements, by default 1 / 60.
            void onFixedUpdate(Timestep _fixedDt);

            /// This method is used to render everything in our game, renderization shouldn't be done other method.
            /// @param _dt The amount of time passed from the previous frame to this current one.
            void onRender(Timestep _dt);

	    public:
	        /// This method allows us to add a renderization layer to the stack.
	        /// @param _layer Layer to add to the stack.
	        /// @sa Layer, LayerStack
            void pushLayer(Layer* _layer);

            /// This method allows us to add a renderization overlay that renders on top of everything to the stack.
            /// @param _layer Layer to add to the stack.
            /// @sa Layer, LayerStack
            void pushOverlay(Layer* _layer);

            /// This method removes a Layer from the stack.
            /// @param _layer Layer to be removed.
            /// @sa Layer, LayerStack
            void popLayer(Layer* _layer);

            /// This method removes an overlay from the stack.
            /// @param _layer Overlay to remove from the stack.
            /// @sa Layer, LayerStack
            void popOverlay(Layer* _layer);

	    public:
	        /// Returns the Window instance.
	        /// @return The main window.
            Window& getWindow() { return *this->window; }

            /// Returns a reference to the application, as it is created as a Singleton.
            /// @return A reference to the application.
            static Application& get() { return *Application::gameInstance; }

	    private:
            bool onWindowClosed(WindowClosedEvent& _e);
            bool onWindowResized(WindowResizedEvent& _e);

	};

	/// Method that MUST be implemented once a class extends Application class. It must return an instance of that
	/// child class.
    Application* createApplication();
}

#endif //APPLICATION_H