#pragma once
#pragma warning( push )
#pragma warning( disable : 4244 )

#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/main/Core.h"
#include "engine/render/Window.h"
#include "engine/event/WindowEvent.h"
#include "engine/util/Timestep.h"
#include "engine/util/Clock.h"
#include "engine/render/layer/Layer.h"
#include "engine/render/layer/LayerStack.h"
#include "engine/render/layer/ImGuiLayer.h"


/*  =================================================================================================
    =                                           GAME                                                =
    =================================================================================================
    = (*) This class is the core of the engine, to use the engine, your application class           =
	=	  must extend this class.
    =                                                                                               =
    = (*) Init:																				        =
    =		- This method is called only once and when the application is initialize, is used		=
	=		  set everything up, load textures, set default values, create objects, position 		=
	=		  the world, load files...																=
	=                                                                                               =
    = (*) HandleEvents:																				=
    = 		- This method is used to called the input of the player, so functions like keyPress		=
	=		  keyDown and KeyUp. Also mouse buttons press, down and up. It is not recomended to		=
	=		  call this methods just listed in the update method as the engine is programmed		=
	=		  to give the best input performance calling these methods on EventHandle.				=
    =                                                                                               =
    = (*) HandleWindowEvents:																		=
	=		- This method takes care of the input from the player realted with the window, such 	=
	=		  us, minimize, resize, close, move... This operations can only be handled in this		=
	=		  method, won't work on other methods, at least not with a predictible beheavour.		=
    =                                                                                               =
    = (*) Update:																					=
    =		- This method is in charge of updating thins like movement (depending on what			=
	=		  movement, if it's based or not on physics), animation updates, GUI updates, AI...		=
	=		  If final user doesn't really care about perfect updates, every update of the game		=
	=		  could be done in this method, and performance won't be affected much, maybe some		=
	=		  visual issues.																		=
	=																								=
	= (*) FixUpdate:																				=
	=		- This method is used to update the physics, and it's better to do it here and not		=
	=		  on the other update methods due to the difference in deltaTime, in this method, 		=
	=		  deltaTime is fixed, so every frame will be updated with the same value, this 			=
	=		  means a more consistent physics emulation.											=
    =                                                                                               =
    = (*) PostUpdate:																				=
    =     	- This method is used to update positions of colliders or other kind of components of	=
	=		  objects that doesn't care about the physics, but only need to know to position to		=
	=		  update to.																			=
    =                                                                                               =
	= (*) Render:																					=
	=		- This methods is in charge of making things to draw on screen once everything on the	=
	=		  game has been correctly updated.														=
	=																								=
	= (*) RenderDebug:																				=
	=		- This method is optional, and you can draw here geometric forms to shape components	=
	=		  and have a visual track of their beheavour.											=
    =                                                                                               =
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
			float dt;
			unsigned int fpsCounter = 0, frameCounter = 0;

	    private:
            bool running = true;
            bool minimized = false;

	    private:
			std::unique_ptr<Window> window;
			static Application* gameInstance;
            LayerStack layerStack;
            ImGuiLayer* imGuiLayer;

            friend int ::main(int argc, char** argv);
            Clock clock;

            void updateFps();

        public:
	        Application();
            virtual ~Application();

            int getFps();
            void setTitle(const std::string& _title);
            void setWindowSize(int _width, int _height);
            void setTimePerFrame(float _timePerFrame) { this->timePerFrame = _timePerFrame; }

            Vec2f getWindowSize() const { return Vec2f(this->window->getWidth(), this->window->getHeight()); }
            std::string& getTitle() const { return this->window->getTitle(); }

            void setVSync(bool _vsync);
            bool isVSync();

            void setFullscreen(bool _fullscreen);

        public:
            bool onWindowClosed(WindowClosedEvent& e);
            bool onWindowResized(WindowResizedEvent& e);
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

	};

    Application* createApplication();
}

#endif //APPLICATION_H

#pragma warning( pop )