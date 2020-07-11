#include "StressTest.h"
#include <imgui.h>


StressTest::StressTest() : engine::Layer("Stress"), cameraController(false), app(Application::get()) {
    this->app.setVSync(false);
}

void StressTest::onInit() {
    this->stressSpriteSheet = Texture2D::create("assets/tests/textures.png");
    this->independentSpriteSheet = Texture2D::create("assets/tests/indepTextures.png");

    for(int _y = 0; _y < 3; _y++) {
        for(int _x = 0; _x < 4; _x++) {
            this->stressSubTextures[_x + 4 * _y] = TextureRegion::create(this->stressSpriteSheet, {(float)_x, (float)_y}, {16, 16});
        }
    }

    for(int _y = 0; _y < 2; _y++) {
        for(int _x = 0; _x < 2; _x++) {
            this->independentTextures[_x + 2 * _y] = TextureRegion::create(this->independentSpriteSheet, {(float)_x, (float)_y}, {81, 56});
        }
    }

    this->cars[0] = GameObject::create(Transform2D({-this->app.getWindowSize().x / 2.f + 81 / 2.f, -162}));
    this->cars[0]->addSprite(this->independentTextures[0]);
    auto _p0 = this->cars[0]->addPhysicsBody();
    _p0->bodyType = BodyType::DYNAMIC;
    _p0->movementType = MovementType::LINEAR;
    _p0->velocity.x = (float)this->independenceInfo.carSpeed0;
    _p0->gravity = {0, 0};

    this->cars[1] = GameObject::create(Transform2D({-this->app.getWindowSize().x / 2.f + 81 / 2.f, -81}));
    this->cars[1]->addSprite(this->independentTextures[1]);
    auto _p1 = this->cars[1]->addPhysicsBody();
    _p1->bodyType = BodyType::DYNAMIC;
    _p1->movementType = MovementType::LINEAR;
    _p1->velocity.x = (float)this->independenceInfo.carSpeed1;
    _p1->gravity = {0, 0};

    this->cars[2] = GameObject::create(Transform2D({-this->app.getWindowSize().x / 2.f + 81 / 2.f, 0}));
    this->cars[2]->addSprite(this->independentTextures[2]);
    auto _p2 = this->cars[2]->addPhysicsBody();
    _p2->bodyType = BodyType::DYNAMIC;
    _p2->movementType = MovementType::LINEAR;
    _p2->velocity.x = (float)this->independenceInfo.carSpeed2;
    _p2->gravity = {0, 0};

    this->cars[3] = GameObject::create(Transform2D({-this->app.getWindowSize().x / 2.f + 81 / 2.f, 81}));
    this->cars[3]->addSprite(this->independentTextures[3]);
    auto _p3 = this->cars[3]->addPhysicsBody();
    _p3->bodyType = BodyType::DYNAMIC;
    _p3->movementType = MovementType::LINEAR;
    _p3->velocity.x = (float)this->independenceInfo.carSpeed3;
    _p3->gravity = {0, 0};

    this->world.addGameObject(this->cars[0], false);
    this->world.addGameObject(this->cars[1], false);
    this->world.addGameObject(this->cars[2], false);
    this->world.addGameObject(this->cars[3], false);

    this->frameBufferPtr = FrameBuffer::create({(uint32_t)this->app.getWindowSize().x, (uint32_t)this->app.getWindowSize().y});
}

void StressTest::onEvent(engine::Event& _e) {
    EventDispatcher _dispatcher(_e);
    _dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(StressTest::mouseScrollEvent));
    if(!_e.handled)
        this->eventInfo.scroll = 0;

    _dispatcher.dispatchEvent<WindowMinimizedEvent>(ENGINE_BIND_EVENT_FN(StressTest::windowMinimizedEvent));
    if(!_e.handled)
        this->eventInfo.minimizing = false;

    _dispatcher.dispatchEvent<WindowMovedEvent>(ENGINE_BIND_EVENT_FN(StressTest::windowMovedEvent));
    if(!_e.handled)
        this->eventInfo.moving = false;

    _dispatcher.dispatchEvent<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(StressTest::windowResizedEvent));
    if(!_e.handled)
        this->eventInfo.resizing = false;
}

void StressTest::onUpdate(engine::Timestep _dt) {
    if(this->tests[2]) {
        for(int _i = 0; _i < 4; _i++)
            this->cars[_i]->getComponentOfType<Sprite>()->update(_dt);
    }

    if(this->tests[2] || this->tests[3]) {
        this->world.update(_dt);
    }

}

void StressTest::onFixedUpdate(engine::Timestep _dt) {
    if(this->tests[2] || this->tests[3]) {
        this->world.fixedUpdate(_dt);
    }
}

void StressTest::onRender(engine::Timestep _dt) {

    Render2D::resetStats();
    RenderCommand::setClearColor(Color::Black);
    RenderCommand::clear();

    if(this->tests[2]) {
        Render2D::beginDraw(this->cameraController.getCamera());
            for(int _i = 0; _i < 4; _i++)
                Render2D::draw(this->cars[_i]);
        Render2D::endDraw();
    }

    if(this->tests[0]) {
        if(this->stressTestInfo.rotate) {
            this->stressTestInfo.rotation += _dt * 360;
            if(this->stressTestInfo.rotation > 360)
                this->stressTestInfo.rotation = 0.0f;
        }
        Render2D::beginDraw(this->cameraController.getCamera());
            for(int _i = 0; _i < this->stressTestInfo.numberOfSprites; _i++) {
                Render2D::drawTexture(this->stressTestInfo.positionsAndTexture[_i].position, {16, 16}, this->stressSubTextures[this->stressTestInfo.positionsAndTexture[_i].textureIndex],
                        this->stressTestInfo.rotation);
            }
        Render2D::endDraw();
    }

    if(this->tests[1]) {

    }

    if(this->tests[2]) {
        for(int _i = 0; _i < 4; _i++) {
            auto _pos = this->cars[_i]->getComponentOfType<PhysicsBody>();
            if(_pos->position.x > this->app.getWindowSize().x / 2.f + 81.f / 2.f)
                _pos->position = Vec2f(-this->app.getWindowSize().x / 2.f + 81.f / 2.f, _pos->position.y);
        }
    }

    if(this->tests[3]) {

    }
}

void StressTest::onImGuiRender(engine::Timestep _dt) {
//        static bool _opened = true;
//    ImGui::ShowDemoWindow(&_opened);

    //    ImGui::Text("FPS(% .1f FPS)", ImGui::GetIO().Framerate);
    //   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration
    ImGui::Begin("Tests Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
    ImGui::Text("FPS %d", this->app.getFps());
    ImGui::SameLine();
    if(ImGui::Checkbox("Vsync", &this->vSync))
        this->app.setVSync(this->vSync);
    if(ImGui::Checkbox("Stress test", &this->tests[0])) {
        if(this->tests[0])
            this->recalculateStressTest();
        else
            this->stressTestInfo.positionsAndTexture.clear();
    }

    ImGui::SameLine();

    ImGui::Checkbox("Independence frame test", &this->tests[2]);

    ImGui::SameLine();

    ImGui::Checkbox("Events test", &this->tests[1]);

    ImGui::SameLine();

    if(ImGui::Checkbox("Physics test test", &this->tests[3])) {

    }

    ImGui::End();

    if(this->tests[0]) {
        ImGui::Begin("Stress", nullptr, ImGuiWindowFlags_NoResize);
            if(ImGui::SliderInt("Images", &this->stressTestInfo.numberOfSprites, 0, 50000)) {
                this->recalculateStressTest();
            }
            ImGui::Checkbox("Rotate", &this->stressTestInfo.rotate);
            if(ImGui::Button("Reset Rotation")) {
                this->stressTestInfo.rotation = 0.0f;
            }
        ImGui::End();
    }

    if(this->tests[1]) {
        ImGui::Begin("Events", nullptr);
//            ImGui::Text("Window moved: %s", this->eventInfo.moving ? "true" : "false");
//            ImGui::Text("Window minimized: %s", this->eventInfo.minimizing ? "true" : "false");
//            ImGui::Text("Window resized: %s", this->eventInfo.resizing ? "true" : "false");
//
//            ImGui::Separator();
//
//            for(auto _button : this->eventInfo.mouseClicks)
//                ImGui::Text("Mouse button: %d", _button);
//
//            ImGui::Text("Mouse position: %f, %f", Input::getMouseX(), Input::getMouseY());
//            ImGui::Text("Mouse scroll: %d", this->eventInfo.scroll);
//
//            ImGui::Separator();
        auto io = ImGui::GetIO();
        ImGui::Text("Mouse delta: (%g, %g)", Input::getMouseX(), Input::getMouseY());
        ImGui::Text("Mouse down: ");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
        ImGui::Text("Mouse wheel: %d", this->eventInfo.scroll);

        ImGui::Separator();

        ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
        ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))             { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
        ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))            { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
        ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");

        ImGui::Separator();

        ImGui::Text("Window minimized: %s", this->eventInfo.minimizing ? "true" : "false");
        ImGui::Text("Window resized: %s", this->eventInfo.resizing ? "true" : "false");
        ImGui::Text("Window moved: %s", this->eventInfo.moving ? "true" : "false");

        ImGui::End();
    }

    if(this->tests[2]) {
        ImGui::Begin("Independence", nullptr, ImGuiWindowFlags_NoResize);
        if(ImGui::SliderInt("Car0", &this->independenceInfo.carSpeed0, 0, 250)) {
            this->cars[0]->getComponentOfType<PhysicsBody>()->velocity.x = (float)this->independenceInfo.carSpeed0;
        }
        if(ImGui::SliderInt("Car1", &this->independenceInfo.carSpeed1, 0, 250)) {
            this->cars[1]->getComponentOfType<PhysicsBody>()->velocity.x = (float)this->independenceInfo.carSpeed1;
        }
        if(ImGui::SliderInt("Car2", &this->independenceInfo.carSpeed2, 0, 250)) {
            this->cars[2]->getComponentOfType<PhysicsBody>()->velocity.x = (float)this->independenceInfo.carSpeed2;
        }
        if(ImGui::SliderInt("Car3", &this->independenceInfo.carSpeed3, 0, 250)) {
            this->cars[3]->getComponentOfType<PhysicsBody>()->velocity.x = (float)this->independenceInfo.carSpeed3;
        }
        ImGui::End();
    }

//    this->eventInfo.restart();
}

void StressTest::onEnd() {
    Layer::onEnd();
}

void StressTest::recalculateStressTest() {
    this->stressTestInfo.positionsAndTexture.clear();

    for(int _i = 0; _i < this->stressTestInfo.numberOfSprites; _i++) {
        this->stressTestInfo.positionsAndTexture.push_back(Position_Index({this->rand.randomf((float)-this->app.getWindowSize().x, (float)this->app.getWindowSize().x / 2),
                                                                                this->rand.randomf((float)-this->app.getWindowSize().y/ 2, (float)this->app.getWindowSize().y/ 2)},
                                                                          this->rand.randomi(0, 11)));
    }

}

bool StressTest::mouseScrollEvent(MouseScrolledEvent& _e) {
    this->eventInfo.scroll = _e.getScrollY();
    return true;
}

bool StressTest::windowResizedEvent(WindowResizedEvent& _e) {
    this->eventInfo.resizing = true;
    return true;
}

bool StressTest::windowMovedEvent(WindowMovedEvent& _e) {
    this->eventInfo.moving = true;
    return true;
}

bool StressTest::windowMinimizedEvent(WindowMinimizedEvent _e) {
    this->eventInfo.minimizing = true;
    return true;
}
