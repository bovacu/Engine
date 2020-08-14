#pragma once

#ifndef ENGINE_INSPECTOR_H
#define ENGINE_INSPECTOR_H

#include <engine/main/Engine.h>

class Inspector : public engine::Layer {
    private:
        engine::OrthographicCameraController    cameraController;
        engine::FrameBufferPtr                  frameBufferPtr  = nullptr;
        engine::Vec2f                           viewportSize    = {0.0f, 0.0f};

        engine::ScenePtr                        scene;
        engine::GameObject                      squareGbj;
        engine::GameObject                      camera;

        bool                                    viewportFocused = false, viewportHovered = false;
        bool                                    playGame = false;
        entt::entity                            gameObjectSelectedInHierarchy = entt::null;

    private:
        void imGuiHierarchy();
        void imGuiComponents();
        void imGuiAssets();
        void imGuiScene();
        void imGuiConsole();
        void imGuiMenuBar();
        void imGuiActionButtonsBar();

    public:
        Inspector();
        ~Inspector()                                override = default;

        void onInit()                               override;
        void onEvent(engine::Event& _e)             override;
        void onUpdate(engine::Delta _dt)            override;
        void onFixedUpdate(engine::Delta _dt)       override;
        void onRender(engine::Delta _dt)            override;
        void onImGuiRender(engine::Delta _dt)       override;
        void onEnd()                                override;



};

#endif //ENGINE_INSPECTOR_H
