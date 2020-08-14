#include <lib/glm/glm/gtc/type_ptr.hpp>
#include "Inspector.h"

Inspector::Inspector() : engine::Layer("Inspector"), cameraController(1280.f / 720.f) {

}

void Inspector::onInit() {
    engine::FrameBufferSpecification _frameBufferSpecification;
    _frameBufferSpecification.width = 1280;
    _frameBufferSpecification.height = 720;
    this->frameBufferPtr = engine::FrameBuffer::create(_frameBufferSpecification);

    this->scene = engine::Scene::create();
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
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
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
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) engine::Application::get().closeApplication();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    ImGui::Text("FPS: %d", engine::Application::get().getFps());

    if (this->squareGbj) {
        ImGui::Separator();
        auto& tag = this->squareGbj.getComponent<engine::Tag>().tag;
        ImGui::Text("%s", tag.c_str());
    }

    ImGui::DragFloat3("Camera Transform",
                      glm::value_ptr(this->camera.getComponent<engine::Transform>().transform[3]));


    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    this->viewportFocused = ImGui::IsWindowFocused();
    this->viewportHovered = ImGui::IsWindowHovered();
//    engine::Application::get().getImGuiLayer()->BlockEvents(!this->viewportFocused || !this->viewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    this->viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint32_t textureID = this->frameBufferPtr->getColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ this->viewportSize.x, this->viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void Inspector::onEnd() {
    Layer::onEnd();
}