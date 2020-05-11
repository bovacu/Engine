#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#include "engine/main/Engine.h"

class TestGame : public engine::Layer {
    private:
        engine::OrthographicCameraController cameraController;

        // Temp
        engine::VertexArrayPtr squareVA;
        engine::ShaderPtr flatColorShader;

        engine::Texture2DPtr checkerboardTexture;

        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

        engine::Texture2DPtr emeraldTextures, playerTexture;
        engine::GameObjectPtr rock;
        engine::GameObjectPtr player;
        engine::World world;

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
