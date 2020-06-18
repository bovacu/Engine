#include "engine/main/Engine.h"
#include "engine/main/EngineExecuter.h"

#include "Safator.h"

class Test : public engine::Application {
    public:
        Test() { engine::Application::pushLayer(new Safator()); }
        ~Test() {  }
};

engine::Application* engine::createApplication() {
    return new Test();
}

