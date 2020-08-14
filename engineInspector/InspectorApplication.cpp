#include "Inspector.h"
#include <engine/main/EngineExecuter.h>

class Game : public engine::Application {
    public:
        Game() { engine::Application::pushLayer(new Inspector()); }
        ~Game() {  }
};

engine::Application* engine::createApplication() {
    return new Game();
}

