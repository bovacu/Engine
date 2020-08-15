#include "Inspector.h"

Inspector::Inspector() : engine::Layer("Inspector"), cameraController(1280.f / 720.f) {
//    engine::Application::get().setVSync(false);
}

void Inspector::onInit() {
    engine::FrameBufferSpecification _frameBufferSpecification;
    _frameBufferSpecification.width = 1280;
    _frameBufferSpecification.height = 720;
    this->frameBufferPtr = engine::FrameBuffer::create(_frameBufferSpecification);

    this->scene = engine::Scene::create("My First Scene");
    this->squareGbj = this->scene->createGameObject("square_gbj");
    this->squareGbj.addComponent<engine::SpriteRenderer>(engine::Color::Green);

    this->camera = this->scene->createGameObject("MainCamera");
    this->camera.addComponent<engine::CameraComponent>();

    class CameraController : public engine::ScriptableObject {
        public:
            void onCreate() {  }

            void onDestroy() {  }

            void onUpdate(engine::Delta _dt) {
                auto& transform = getComponent<engine::Transform>().transform;
                float speed = 5.0f;

                if (engine::Input::isKeyPressed(engine::KeyCode::A))
                    transform[3][0] += speed * _dt;
                if (engine::Input::isKeyPressed(engine::KeyCode::D))
                    transform[3][0] -= speed * _dt;
                if (engine::Input::isKeyPressed(engine::KeyCode::W))
                    transform[3][1] -= speed * _dt;
                if (engine::Input::isKeyPressed(engine::KeyCode::S))
                    transform[3][1] += speed * _dt;
            }
    };

    class Dummy : public engine::ScriptableObject {
        public:
            void onCreate() {  }

            void onDestroy() {  }

            void onUpdate(engine::Delta _dt) {

            }
    };

    this->camera.addComponent<engine::NativeScript>().bind<CameraController>();
}

void Inspector::onEvent(engine::Event& _e) {
    this->cameraController.onEvent(_e);
}

void Inspector::onUpdate(engine::Delta _dt) {
    auto _fbSpec = this->frameBufferPtr->getSpecification();
    if(this->viewportSize.x > 0.0f && this->viewportSize.y > 0.0f && (_fbSpec.width != this->viewportSize.x || _fbSpec.height != this->viewportSize.y)) {
        this->frameBufferPtr->resize((uint32_t)this->viewportSize.x, (uint32_t)this->viewportSize.y);
        this->cameraController.onResize(this->viewportSize.x, this->viewportSize.y);
        this->scene->onViewportResize((int)this->viewportSize.x, (int)this->viewportSize.y);
    }

    if(this->playGame)
        this->scene->onUpdate(_dt);
}

void Inspector::onFixedUpdate(engine::Delta _dt) {
    Layer::onFixedUpdate(_dt);
}

void Inspector::onRender(engine::Delta _dt) {
    if(this->viewportFocused)
        this->cameraController.onUpdate(_dt);

    this->frameBufferPtr->bind();
        engine::Renderer::setClearColor(engine::Color(25, 25, 25, 255));
        engine::Renderer::clear();
        this->scene->onRender(_dt);
    this->frameBufferPtr->unbind();
}

void Inspector::onImGuiRender(engine::Delta _dt) {
//    ImGui::ShowDemoWindow(nullptr);
    // Note: Switch this to true to enable dockspace

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    /// This is the general dockspace, and it is set to not be visible.
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        this->imGuiMenuBar();
        this->imGuiActionButtonsBar();
    ImGui::PopStyleVar();
            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            this->imGuiHierarchy();
            this->imGuiComponents();
            this->imGuiConsole();
            this->imGuiScene();
            this->imGuiAssets();
        ImGui::End();
}

void Inspector::onEnd() {
    Layer::onEnd();
}

void Inspector::imGuiMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit"))
                engine::Application::get().closeApplication();
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Edit")) {

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Help")) {

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
void Inspector::imGuiActionButtonsBar() {
    const int _width = 50;
    ImGui::SetNextItemWidth(_width);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - _width);
    if(ImGui::Button("Play")) {
        this->playGame = !this->playGame;
    }

    ImGui::SameLine();

    if(ImGui::Button("Stop")) {
        this->playGame = false;
    }

    if(this->playGame) {
        ImGui::SameLine();
        ImGui::TextColored({0.f, 1.f, 0.f, 1.f}, "Playing game");
    }
}
void Inspector::imGuiHierarchy() {
    ImGui::Begin("Hierarchy", nullptr);
        if (ImGui::TreeNode("scene", "%s", this->scene->getName().c_str())) {
            static int _selected = -1;
            int _n = 0;
            auto _view = this->scene->getGameObjectsRegistry().view<engine::Tag>();
            for (auto _gameObject : _view) {
                char buf[32];
                #if defined(ENGINE_PLATFORM_WINDOWS)
                    sprintf_s(buf, "%s", (&_view.get<engine::Tag>(_gameObject).tag)->c_str());
                #elif defined(ENGINE_PLATFORM_LINUX)
                    sprintf(buf, "%s", (&_view.get<engine::Tag>(_gameObject).tag)->c_str());
                #endif
                if (ImGui::Selectable(buf, _selected == _n)) {
                    _selected = _n;
                    this->gameObjectSelectedInHierarchy = _gameObject;
                }

                _n++;
            }

            ImGui::TreePop();
        }
    ImGui::End();
}
void Inspector::imGuiComponents() {
    ImGui::Begin("Components", nullptr);

    auto& _registry = this->scene->getGameObjectsRegistry();

    auto _tagView = _registry.view<engine::Tag>();
    for(auto _gameObject : _tagView) {
        if(_gameObject == this->gameObjectSelectedInHierarchy) {

            auto& _tag = _tagView.get<engine::Tag>(_gameObject);

            ImGui::Text("Tag: "); ImGui::SameLine();
            char _myTag[256];
            strcpy_s(_myTag, _tag.tag.c_str());
            ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
            ImGui::InputText("##myText", _myTag, 256);
            if(!ImGui::IsItemActive()&& strcmp((char*)_tag.tag.c_str(), _myTag) != 0)
                _tag.tag = _myTag;

            ImGui::Separator();
            break;
        }

    }

    auto _transformView = _registry.view<engine::Transform>();
    for(auto _gameObject : _transformView) {
        auto& _transform = _transformView.get<engine::Transform>(_gameObject);

        if(_gameObject == this->gameObjectSelectedInHierarchy) {
            if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Position "); ImGui::SameLine();

                float _x = _transform.getPosition2D().x;
                ImGui::Text("X "); ImGui::SameLine();
                ImGui::SetNextItemWidth(50);
                ImGui::PushID(0);
                    if(ImGui::DragFloat("##myInput", &_x, 0.5f))
                        _transform.setX(_x);
                ImGui::PopID();

                ImGui::SameLine();

                float _y = _transform.getPosition2D().y;
                ImGui::Text("Y "); ImGui::SameLine();
                ImGui::SetNextItemWidth(50);
                ImGui::PushID(1);
                    if(ImGui::DragFloat("##myInput", &_y, 0.5f))
                        _transform.setY(_y);
                ImGui::PopID();
            }

            break;
        }
    }

    auto _cameraView = _registry.view<engine::CameraComponent>();
    for(auto _gameObject : _cameraView) {
        if(_gameObject == this->gameObjectSelectedInHierarchy) {
            auto& _camera = _cameraView.get<engine::CameraComponent>(_gameObject);
            if (ImGui::CollapsingHeader("Camera")) {
                float _near = _camera.sceneCamera.getNear();
                ImGui::Text("Near "); ImGui::SameLine();
                ImGui::SetNextItemWidth(50);
                ImGui::PushID(2);
                    if(ImGui::DragFloat("##myInput", &_near, 0.5f))
                        _camera.sceneCamera.setOrthographic(_camera.sceneCamera.getOrthographicSize(), _near, _camera.sceneCamera.getFar());
                ImGui::PopID();

                ImGui::SameLine();

                float _far = _camera.sceneCamera.getFar();
                ImGui::Text("Far "); ImGui::SameLine();
                ImGui::SetNextItemWidth(50);
                ImGui::PushID(3);
                    if(ImGui::DragFloat("##myInput", &_far, 0.5f))
                        _camera.sceneCamera.setOrthographic(_camera.sceneCamera.getOrthographicSize(), _camera.sceneCamera.getNear(), _far);
                ImGui::PopID();

                float _size = _camera.sceneCamera.getOrthographicSize();
                ImGui::Text("Size "); ImGui::SameLine();
                ImGui::SetNextItemWidth(50);
                ImGui::PushID(4);
                    if(ImGui::DragFloat("##myInput", &_size, 0.5f))
                        _camera.sceneCamera.setOrthographic(_size, _camera.sceneCamera.getNear(), _camera.sceneCamera.getFar());
                ImGui::PopID();
            }
            break;
        }

    }

    auto _spriteRendererView = _registry.view<engine::SpriteRenderer>();
    for(auto _gameObject : _spriteRendererView) {
        if(_gameObject == this->gameObjectSelectedInHierarchy) {
            auto& _sprite = _spriteRendererView.get<engine::SpriteRenderer>(_gameObject);
            if (ImGui::CollapsingHeader("Sprite Renderer"))
                this->imGuiColorPicker(_sprite.color);
            break;
        }

    }

    auto _nativeScriptsView = _registry.view<engine::NativeScript>();
    for(auto _gameObject : _nativeScriptsView) {
        if(_gameObject == this->gameObjectSelectedInHierarchy) {
            auto& _nativeScript = _nativeScriptsView.get<engine::NativeScript>(_gameObject);
            if (ImGui::CollapsingHeader(_nativeScript.className.c_str())) {
                ImGui::TextColored({0.f, 1.f, 0.f, 1.f}, "We have here a script!");
            }
        }

    }

    ImGui::End();
}
void Inspector::imGuiAssets() {
    ImGui::Begin("Assets", nullptr);

    ImGui::End();
}
void Inspector::imGuiScene() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Scene", nullptr);
            this->viewportFocused = ImGui::IsWindowFocused();
            this->viewportHovered = ImGui::IsWindowHovered();
            //    engine::Application::get().getImGuiLayer()->BlockEvents(!this->viewportFocused || !this->viewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            this->viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            uint32_t textureID = this->frameBufferPtr->getColorAttachmentRendererID();
            ImGui::Image((void*)(intptr_t)textureID, ImVec2{ this->viewportSize.x, this->viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
    ImGui::PopStyleVar();

}
void Inspector::imGuiConsole() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {ImGui::GetStyle().WindowPadding.x / 2, 0});
        ImGui::Begin("Console", nullptr);
    ImGui::PopStyleVar();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
                    ImGui::Button("Clear"); ImGui::SameLine();
                    ImGui::Button("Stop");
                ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            ImGui::Separator();

        ImGui::End();
}
void Inspector::imGuiColorPicker(engine::Color& _color) {
    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar;
    static float _newColor[4] = {(float)_color.r / 255.f, (float)_color.g / 255.f, (float)_color.b / 255.f, (float)_color.a / 255.f };
    static bool saved_palette_init = true;
    static ImVec4 saved_palette[32] = {};
    if (saved_palette_init) {
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++) {
            ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
            saved_palette[n].w = 1.0f; // Alpha
        }
        saved_palette_init = false;
    }

    static ImVec4 backup_color;
    const int _paletteButtonWidth = 60;
    bool open_popup = ImGui::ColorButton("MyColor##3b", {_newColor[0], _newColor[1], _newColor[2], _newColor[3]}, misc_flags | ImGuiColorEditFlags_NoTooltip,
                                         {ImGui::GetWindowContentRegionWidth() - _paletteButtonWidth, 0});
    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
    open_popup |= ImGui::Button("Palette", {_paletteButtonWidth, 0});
    if (open_popup) {
        ImGui::OpenPopup("mypicker");
        backup_color = {_newColor[0], _newColor[1], _newColor[2], _newColor[3]};
    }
    if (ImGui::BeginPopup("mypicker")) {
        ImGui::Text("Color picker");
        ImGui::Separator();
        if(ImGui::ColorPicker4("##picker", _newColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview)) {
            _color.r = (unsigned char)(_newColor[0] * 255);
            _color.g = (unsigned char)(_newColor[1] * 255);
            _color.b = (unsigned char)(_newColor[2] * 255);
            _color.a = (unsigned char)(_newColor[3] * 255);
        }

        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
            ImGui::Text("Current");
            ImGui::ColorButton("##current", {_newColor[0], _newColor[1], _newColor[2], _newColor[3]}, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40));
            ImGui::Separator();
            ImGui::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++) {
                ImGui::PushID(n);
                if ((n % 8) != 0)
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
                ImGui::ColorButton("##palette", saved_palette[n], ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20,20));

                // Allow user to drop colors into each palette entry
                // (Note that ColorButton is already a drag source by default, unless using ImGuiColorEditFlags_NoDragDrop)
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    ImGui::EndDragDropTarget();
                }

                ImGui::PopID();
            }
            ImGui::EndGroup();
        ImGui::EndPopup();
    }
}
