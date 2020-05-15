#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#define MAX_BRUSH_THICKNESS 20

#define SAND_COLOR          { 194, 178, 128, 255 }
#define WATER_COLOR         {  90, 188, 216, 125 }
#define TRANSPARENT_COLOR   {   0,   0,   0,   0 }
#define ROCK_COLOR          { 187, 182, 177, 255 }

#include <engine/main/Engine.h>
#include <random>

using namespace engine;

class TestGame : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE, SAND, WATER, ROCK };

        struct Particle {
            Vec2f position;
            Color color;
            ParticleType type;
            bool updated = false;
        };

    private:
        engine::OrthographicCameraController cameraController;
        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        Texture2DPtr proceduralTexture;
        ImGuiTexture2DPtr pauseTexture, resumeTexture, advanceTexture;
        Particle* particles;
        Application& app;

        bool play = true, oneStep = false;
        ParticleType selectedParticle = SAND;
        int brushSize = 1;

        std::random_device rd;
        std::mt19937 mt;

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

        Color particleTypeToColor(const ParticleType& _particle);
        void generateParticles(const Vec2f& _mousePos);
        void generateWithBrush(const Vec2f _mousePos);
};

#endif //TESTGAME_H
