#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#include "engine/main/Engine.h"

class TestGame : public engine::Layer {
    private:
        engine::OrthographicCameraController cameraController;

        // Temp
        std::shared_ptr<engine::VertexArray> squareVA;
        std::shared_ptr<engine::Shader> flatColorShader;

        std::shared_ptr<engine::Texture2D> checkerboardTexture;

        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

    public:
        TestGame();
        ~TestGame() override = default;

        void onInit()                               override;
        void onEvent(engine::Event& _e)             override;
        void onUpdate(engine::Timestep _dt)         override;
        void onFixedUpdate(engine::Timestep _dt)    override;
        void onRender(engine::Timestep _dt)         override;
        void onImGuiRender(engine::Timestep _dt)    override;
        void onEnd()                                override;
};

#endif //TESTGAME_H
