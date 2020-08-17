#include "Inspector.h"

#if defined(USING_INSPECTOR)
    Inspector::InspectorLogger Inspector::logger;
#endif

Inspector::Inspector() : engine::Layer("Inspector"), cameraController(1280.f / 720.f) {

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
            void onCreate() {
                engine::api::Console::log("Camera Controller Script created");
            }

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
    this->camera.addComponent<engine::NativeScript>().bind<CameraController>();

    this->dummy = this->scene->createGameObject("DummyRectangle");

    class Dummy : public engine::ScriptableObject {
        public:
            void onCreate() {
                this->getComponent<engine::Transform>().setX(5);
                auto& _sr = this->addComponent<engine::SpriteRenderer>();
                _sr.color = engine::Color::Red;
            }

            void onDestroy() {  }

            void onUpdate(engine::Delta _dt) {

            }
    };

    this->dummy.addComponent<engine::NativeScript>().bind<Dummy>();
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
        if(!this->playGame) {
            Inspector::logger.clear();
//            this->scene->getGameObjectsRegistry().view<engine::NativeScript>().each([=] (auto _gameObject, auto& _nativeScript) {
//                if(_nativeScript.scriptableObject)
//                    _nativeScript.destroy();
//            });
        }
        this->playGame = !this->playGame;
    }

    ImGui::SameLine();

    if(ImGui::Button("Stop"))
        this->playGame = false;

    if(this->playGame) {
        ImGui::SameLine();
        ImGui::TextColored({0.f, 1.f, 0.f, 1.f}, "Playing game");
    }
}
void Inspector::imGuiHierarchy() {
    ImGui::Begin("Hierarchy", nullptr);

        if(ImGui::Button("Add"))
            this->gameObjectSelectedInHierarchy = (entt::entity)this->scene->createGameObject().getRef();

        ImGui::SameLine();

        if(ImGui::Button("Remove")) {
            this->scene->getGameObjectsRegistry().destroy(this->gameObjectSelectedInHierarchy);
            this->gameObjectSelectedInHierarchy = entt::null;
        }

        static ImGuiTextFilter _filter;
        _filter.Draw("Search", ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize("Search").x);

        ImGui::Separator();

        if (ImGui::CollapsingHeader(this->scene->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            static int _selected = -1;
            int _n = 0;
            auto _view = this->scene->getGameObjectsRegistry().view<engine::Active, engine::Tag>();
            for (auto _gameObject : _view) {
                auto _objectName = (&_view.get<engine::Tag>(_gameObject).tag)->c_str();
                if (_filter.PassFilter(_objectName)) {
                    bool _active = _view.get<engine::Active>(_gameObject).active;
                    auto _flags = _active ? ImGuiSelectableFlags_None : ImGuiSelectableFlags_DisabledColor;
                    if (ImGui::Selectable(_objectName, _gameObject == this->gameObjectSelectedInHierarchy, _flags)) {
                        _selected = _n;
                        this->gameObjectSelectedInHierarchy = _gameObject;
                    }
                }
            }
        }

    ImGui::End();
}
void Inspector::imGuiComponents() {
    ImGui::Begin("Components", nullptr);

    if(this->gameObjectSelectedInHierarchy != entt::null) {
        auto& _registry = this->scene->getGameObjectsRegistry();

        auto _tagView = _registry.group<engine::Tag>(entt::get<engine::Active>);
        for(auto _gameObject : _tagView) {
            if(_gameObject == this->gameObjectSelectedInHierarchy) {
                if(ImGui::CollapsingHeader("Tag")) {

                    auto& _active = _tagView.get<engine::Active>(_gameObject);
                    ImGui::PushID(0);
                    ImGui::Checkbox("Active", &_active.active);
                    ImGui::PopID();

                    auto& _tag = _tagView.get<engine::Tag>(_gameObject);
                    ImGui::Text("Tag: "); ImGui::SameLine();
                    char _myTag[256];
                    strcpy_s(_myTag, _tag.tag.c_str());
                    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                    ImGui::InputText("##myText", _myTag, 256);
                    if(!ImGui::IsItemActive()&& strcmp((char*)_tag.tag.c_str(), _myTag) != 0)
                        _tag.tag = _myTag;
                }

                ImGui::Separator();
                break;
            }

        }

        auto _transformView = _registry.view<engine::Transform>();
        for(auto _gameObject : _transformView) {
            auto& _transform = _transformView.get<engine::Transform>(_gameObject);

            if(_gameObject == this->gameObjectSelectedInHierarchy) {
                if (ImGui::CollapsingHeader("Transform")) {
                    /// -------------- Position
                    auto _size = ImGui::CalcTextSize("Position ");
                    ImGui::Text("Position "); ImGui::SameLine(75);
                    float _xPosOfCoordinateX = 0;

                    float _x = _transform.getPosition2D().x;
                    ImGui::Text("X "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(1);
                    if(ImGui::DragFloat("##myInput", &_x, 0.5f))
                        _transform.setX(_x);
                    _xPosOfCoordinateX = ImGui::GetCursorPosX();
                    ImGui::PopID();

                    ImGui::SameLine();

                    float _y = _transform.getPosition2D().y;
                    ImGui::Text("Y "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(2);
                    if(ImGui::DragFloat("##myInput", &_y, 0.5f))
                        _transform.setY(_y);
                    ImGui::PopID();

                    /// -------------- Scale
                    ImGui::Text("Scale "); ImGui::SameLine(75);

                    float _scaleX = _transform.getScale2D().x;
                    ImGui::Text("X "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(3);
                    if(ImGui::DragFloat("##myInput", &_scaleX, 0.25f))
                        _transform.setScaleX(_scaleX >= 0 ? _scaleX : _transform.getScaleX());
                    ImGui::PopID();

                    ImGui::SameLine();

                    float _scaleY = _transform.getScale2D().y;
                    ImGui::Text("Y "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(4);
                    if(ImGui::DragFloat("##myInput", &_scaleY, 0.25f))
                        _transform.setScaleY(_scaleY >= 0 ? _scaleY : _transform.getScaleY());
                    ImGui::PopID();
                }

                ImGui::Separator();
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
                    ImGui::PushID(5);
                    if(ImGui::DragFloat("##myInput", &_near, 0.5f))
                        _camera.sceneCamera.setOrthographic(_camera.sceneCamera.getOrthographicSize(), _near, _camera.sceneCamera.getFar());
                    ImGui::PopID();

                    ImGui::SameLine();

                    float _far = _camera.sceneCamera.getFar();
                    ImGui::Text("Far "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(6);
                    if(ImGui::DragFloat("##myInput", &_far, 0.5f))
                        _camera.sceneCamera.setOrthographic(_camera.sceneCamera.getOrthographicSize(), _camera.sceneCamera.getNear(), _far);
                    ImGui::PopID();

                    float _size = _camera.sceneCamera.getOrthographicSize();
                    ImGui::Text("Size "); ImGui::SameLine();
                    ImGui::SetNextItemWidth(50);
                    ImGui::PushID(7);
                    if(ImGui::DragFloat("##myInput", &_size, 0.5f))
                        _camera.sceneCamera.setOrthographic(_size, _camera.sceneCamera.getNear(), _camera.sceneCamera.getFar());
                    ImGui::PopID();

                    ImGui::PushID(8);
                        if(ImGui::Button("Remove")) {
                            this->scene->getGameObjectsRegistry().remove<engine::CameraComponent>(this->gameObjectSelectedInHierarchy);
                        }
                    ImGui::PopID();
                }

                ImGui::Separator();
                break;
            }
        }

        auto _spriteRendererView = _registry.view<engine::SpriteRenderer>();
        for(auto _gameObject : _spriteRendererView) {
            if(_gameObject == this->gameObjectSelectedInHierarchy) {
                auto& _sprite = _spriteRendererView.get<engine::SpriteRenderer>(_gameObject);
                if (ImGui::CollapsingHeader("Sprite Renderer")) {
                    ImGui::PushID(9);
                        this->imGuiColorPicker(_sprite.color);
                        if(ImGui::Button("Remove")) {
                            this->scene->getGameObjectsRegistry().remove<engine::SpriteRenderer>(this->gameObjectSelectedInHierarchy);
                        }
                    ImGui::PopID();
                }

                ImGui::Separator();
                break;
            }
        }

        auto _nativeScriptsView = _registry.view<engine::NativeScript>();
        for(auto _gameObject : _nativeScriptsView) {
            if(_gameObject == this->gameObjectSelectedInHierarchy) {
                auto& _nativeScript = _nativeScriptsView.get<engine::NativeScript>(_gameObject);
                if (ImGui::CollapsingHeader(_nativeScript.className.c_str())) {
                    ImGui::TextColored({0.f, 1.f, 0.f, 1.f}, "We have here a script!");
                    ImGui::PushID(10);
                    if(ImGui::Button("Remove")) {
                        this->scene->getGameObjectsRegistry().remove<engine::SpriteRenderer>(this->gameObjectSelectedInHierarchy);
                    }
                    ImGui::PopID();
                }

                ImGui::Separator();
                break;
            }
        }

        if(ImGui::Button("Add Component", {ImGui::GetWindowContentRegionWidth(), 0.0f})) {
            ImGui::OpenPopup("Add Component");
            ImGui::SetNextWindowSize({250, 300});
        }

        this->imGuiAddComponentWindow();
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
                    if(ImGui::Button("Clear")) { Inspector::logger.clear(); } ImGui::SameLine();
                    ImGui::Button("Stop");
                ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            ImGui::Separator();

    #if defined(USING_INSPECTOR)
        Inspector::logger.draw("Console");
    #endif

        ImGui::End();
}
void Inspector::imGuiColorPicker(engine::Color& _color) {
    ImGuiColorEditFlags misc_flags = (unsigned )ImGuiColorEditFlags_AlphaPreview | (unsigned )ImGuiColorEditFlags_AlphaBar;

    float _newColor[4] = {(float)_color.r / 255.f, (float)_color.g / 255.f, (float)_color.b / 255.f, (float)_color.a / 255.f };
    const int _paletteButtonWidth = 60;

    bool open_popup = ImGui::ColorButton("MyColor##3b", {_newColor[0], _newColor[1], _newColor[2],
                                         _newColor[3]}, misc_flags | ImGuiColorEditFlags_NoTooltip,
                                         {ImGui::GetWindowContentRegionWidth() - _paletteButtonWidth, 0});

    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
    open_popup |= ImGui::Button("Palette", {_paletteButtonWidth, 0});

    if (open_popup)
        ImGui::OpenPopup("mypicker");

    if (ImGui::BeginPopup("mypicker")) {
        ImGui::Text("Color picker");
        ImGui::Separator();
        if(ImGui::ColorPicker4("##picker", _newColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar)) {
            _color.r = (unsigned char)(_newColor[0] * 255);
            _color.g = (unsigned char)(_newColor[1] * 255);
            _color.b = (unsigned char)(_newColor[2] * 255);
            _color.a = (unsigned char)(_newColor[3] * 255);
        }

        ImGui::EndPopup();
    }
}

void Inspector::imGuiAddComponentWindow() {
    bool _open = true;
    if(ImGui::BeginPopupModal("Add Component", &_open)) {

        const static char* _components[] = {"Sprite Renderer", "Camera", "Native Script"};

        static int selected = -1;
        for (int _i = 0; _i < 3; _i++) {
            char _buf[32];
            #if defined(ENGINE_PLATFORM_WINDOWS)
                sprintf_s(_buf, "%s", _components[_i]);
            #elif defined(ENGINE_PLATFORM_LINUX)
                sprintf(_buf, "%s", _components[_i]);
            #endif

            if (ImGui::Selectable(_buf, selected == _i)) {
                selected = _i;
                switch (_i) {
                    case 0 : {
                        this->scene->getGameObjectsRegistry().emplace<engine::SpriteRenderer>(this->gameObjectSelectedInHierarchy, engine::Color::White);
                        ImGui::CloseCurrentPopup();
                        break;
                    }

                    case 1 : {
                        this->scene->getGameObjectsRegistry().emplace<engine::Camera>(this->gameObjectSelectedInHierarchy);
                        ImGui::CloseCurrentPopup();
                        break;
                    }

                    case 2 : {
                        this->scene->getGameObjectsRegistry().emplace<engine::NativeScript>(this->gameObjectSelectedInHierarchy);
                        ImGui::CloseCurrentPopup();
                        break;
                    }

                    default: break;
                }
            }
        }

        ImGui::EndPopup();
    }
}
