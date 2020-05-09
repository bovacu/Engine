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
    =		- This method is used to update the objects and also where input should be caught.      =
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
    = (*) The engine allows you to run the main loop in two ways, full speed, but getting a 		=
	=	  usable delta. It's not enabled by default, and the way to enable it is using the method	=
	=	  stuckFpsTo(NO_LIMIT_FPS).																	=
	=																								=
	=	  The other way is to limit the FPS of the game to the player, and you have to call the 	=
	=	  same method with any FPS_X defined macro or directly passing the value as an int. It 		=
	=	  is not recommended to set a huge amount of FPS, in that case just use the first way.		=
	=																								=
	= (*) The engine also allows you to keep track of the FPS that the game is running at with 		=
	=	  the method getGameFps().																	= 
    =                                                                                               =
    ================================================================================================= 
*/


#define NO_FPS_LIMIT -1
#define FPS_15		 15
#define FPS_30 		 30
#define FPS_60 		 60
#define FPS_90 		 90
#define FPS_120		 120
#define FPS_150		 150
#define FPS_180		 180
#define FPS_200		 200

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

            friend int ::main(int argc, char** argv);
            void updateFps();

        public:
	        Application();
            virtual ~Application();

            int getFps();
            void setTitle(const std::string& _title);
            [[nodiscard]] const std::string& getTitle() const   { return this->window->getTitle(); }

            void setWindowSize(int _width, int _height);
            [[nodiscard]] Vec2f getWindowSize() const           { return Vec2f((float)this->window->getWidth(), (float)this->window->getHeight()); }

            void setTimePerFrame(float _timePerFrame)           { this->timePerFrame = _timePerFrame; }
            [[nodiscard]] float getTimePerFrame() const         { return this->timePerFrame; }

            void setVSync(bool _vsync);
            bool isVSync();

            [[nodiscard]] Timestep getDelta() const             { return this->dt; }

            void setFullscreen(bool _fullscreen);
            [[nodiscard]] bool isFullscreen() const             {  return this->window->isFullscreen(); }

        public:
            void onRun();
            void onEvent(Event& _event);
            void onUpdate(Timestep _dt);
            void onFixedUpdate(Timestep _fixedDt);
            void onRender(Timestep _dt);

	    public:
            void pushLayer(Layer* _layer);
            void pushOverlay(Layer* _layer);


	    public:
            Window& getWindow() { return *this->window; }
            static Application& get() { return *Application::gameInstance; }

	    private:
            bool onWindowClosed(WindowClosedEvent& _e);
            bool onWindowResized(WindowResizedEvent& _e);

	};

    Application* createApplication();
}

#endif //APPLICATION_H