#include "pch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <engine/main/Application.h>
#include <GLFW/glfw3.h> // TEMPORARY

#include <engine/render/Render2D.h>

namespace engine {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {  }

    void ImGuiLayer::onInit() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& _game = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(_game.getWindow().getNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onEvent(Event& _e) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(ImGuiLayer::onMouseScrolled));
        dispatcher.dispatchEvent<MouseButtonPressedEvent>(ENGINE_BIND_EVENT_FN(ImGuiLayer::onMouseClicked));
        dispatcher.dispatchEvent<MouseMovedEvent>(ENGINE_BIND_EVENT_FN(ImGuiLayer::onMouseMovedEvent));
    }

    bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent& _e) {
        return ImGui::IsAnyWindowHovered();
    }

    void ImGuiLayer::onEnd() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end() {
        ImGuiIO& io = ImGui::GetIO();
        Application& _game = Application::get();
        io.DisplaySize = ImVec2((float)_game.getWindow().getWidth(), (float)_game.getWindow().getHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::drawDebugInfo() {
        static bool _vsync = Application::get().isVSync(), _fullscreen = false;
        static int _fps = 60;
        static int _windowRes[2] = {(int) engine::Application::get().getWindowSize().x,
                                    (int) engine::Application::get().getWindowSize().y};

        static const char* _resSelected = "800x480";

        ImGui::Begin("Settings");
        auto stats = engine::Render2D::getStats();
        ImGui::Text("FPS: %d", engine::Application::get().getFps()); ImGui::SameLine();
        if(ImGui::SliderInt("", &_fps, 30, 300)) {
            engine::Application::get().setTimePerFrame(1.f / _fps);
        }

        if(ImGui::Checkbox("VSync Active", &_vsync)) {
            engine::Application::get().getWindow().setVSync(_vsync);
        }

        if(ImGui::Checkbox("Fullscreen", &_fullscreen)) {
            engine::Application::get().getWindow().setFullscreen(_fullscreen);
        }

        const char* _resolutions[] = { "2560x1440", "1920x1080", "1366x768", "1280x720", "1920x1200", "1680x1050",
                                       "1440x900" ,"1280x800" ,"1024x768" ,"800x600", "800x480","640x480", "320x240"
        };

        ImGui::Text("Resolution"); ImGui::SameLine();
        if (ImGui::BeginCombo("##combo", _resSelected)){ // The second parameter is the label previewed before opening the combo. {
            for (auto & _resolution : _resolutions) {
                bool is_selected = (_resSelected == _resolution); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(_resolution, is_selected)) {
                    _resSelected = _resolution;
                    charToIntSize(std::string(_resolution), _windowRes);
                    engine::Application::get().setWindowSize(_windowRes[0], _windowRes[1]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        ImGui::Text("Render2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());
        ImGui::End();
    }

    void ImGuiLayer::charToIntSize(const std::string& _size, int* _resolution) {
        size_t pos;
        std::string _token;
        std::string _delimiter = "x";
        pos = _size.find(_delimiter);
        int _width = atoi(_size.substr(0, pos).c_str());
        int _height = atoi( _size.substr(pos + _delimiter.length()).c_str());

        _resolution[0] = _width;
        _resolution[1] = _height;
    }

    bool ImGuiLayer::onMouseClicked(MouseButtonPressedEvent& _e) {
        return ImGui::IsAnyWindowHovered();
    }

    bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& _e) {
        return ImGui::IsAnyWindowHovered();
    }
}