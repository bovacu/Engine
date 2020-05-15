#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#include <engine/main/Engine.h>

using namespace engine;

class TestGame : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE, SAND, WATER };

        struct Particle {
            Vec2f position;
            Color color;
            ParticleType type;
            int posInVector;
            bool updated = false;
        };

    private:
        engine::OrthographicCameraController cameraController;
        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        Texture2DPtr proceduralTexture;
        Particle* particles;
        Application& app;

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

    private:
        void initSimulationWorld();
        void updateSandParticle(int _pos);
        void updateWaterParticle(int _pos);

        bool downNeighbour(int _pos);
        bool leftNeighbour(int _pos);
        bool rightNeighbour(int _pos);
        bool upNeighbour(int _pos);
        bool downLeftNeighbour(int _pos);
        bool downRightNeighbour(int _pos);
};

#endif //TESTGAME_H
