#include "Safator.h"
#include <engine/main/EngineExecuter.h>

class Game : public engine::Application {
    public:
        Game() { engine::Application::pushLayer(new Safator()); }
        ~Game() {  }
};

engine::Application* engine::createApplication() {
    return new Game();
}

