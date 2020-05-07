#include "engine/main/Engine.h"
#include "engine/main/EngineExecuter.h"

#include "TestGame.h"

class Test : public engine::Application {
    public:
        Test() { engine::Application::pushLayer(new TestGame()); }
        ~Test() {  }
};

engine::Application* engine::createApplication() {
    return new Test();
}

