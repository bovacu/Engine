#if defined(COMPILE_TEST)
#include "engine/main/Engine.h"
#include "engine/main/EngineExecuter.h"

#include "StressTest.h"

class Stress : public engine::Application {
    public:
        Stress() { engine::Application::pushLayer(new StressTest()); }
        ~Stress() {  }
};

engine::Application* engine::createApplication() {
    return new Stress();
}
#endif