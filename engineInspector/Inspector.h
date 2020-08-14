#pragma once

#ifndef ENGINE_INSPECTOR_H
#define ENGINE_INSPECTOR_H

#include <engine/main/Engine.h>

class Inspector : public engine::Layer {
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

    private:
        engine::OrthographicCameraController    cameraController;
        engine::FrameBufferPtr                  frameBufferPtr  = nullptr;
        engine::Vec2f                           viewportSize    = {0.0f, 0.0f};

        engine::ScenePtr                        scene;
        engine::GameObject                      squareGbj;
        engine::GameObject                      camera;

        bool                                    viewportFocused = false, viewportHovered = false;

};

#endif //ENGINE_INSPECTOR_H
