#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#define MAX_BRUSH_THICKNESS 20

#define SAND_COLOR          { 194, 178, 128, 255 }
#define WATER_COLOR         {  90, 188, 216, 125 }
#define TRANSPARENT_COLOR   {   0,   0,   0,   0 }
#define ROCK_COLOR          { 187, 182, 177, 255 }

#include <engine/main/Engine.h>

using namespace engine;

class TestGame : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE, SAND, WATER, ROCK };
        enum Tool { DRAW, ERASE };
        float gravity = 10.f;

        struct Particle {
            Vec2f velocity  {0.0f, 0.0f};
            Vec2i direction {  0,    0 };
            Color color;
            ParticleType type;
            bool updated = false;
        };

    private:
        engine::OrthographicCameraController cameraController;
        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        Texture2DPtr proceduralTexture;
        ImGuiTexture2DPtr pauseTexture, resumeTexture, advanceTexture, oneFrameTexture, drawTexture, eraseTexture;
        Particle* particles;
        Application& app;

        bool play = true, oneStep = false;
        ParticleType selectedParticle = SAND;

        Tool usingTool = DRAW;
        int brushSize = 10;

        engine::Random random;

        int particlesUpdating = 0;

        Color PARTICLE_COLORS[8] = {
                { 202, 188, 145, 255 }, /// SAND_0
                { 194, 178, 128, 255 }, /// SAND_1
                { 186, 168, 111, 255 }, /// SAND_2
                { 177, 157,  94, 255 }, /// SAND_3
                { 166, 145,  80, 255 }, /// SAND_4
                {  90, 188, 216, 125 }, /// WATER_0
                { 187, 182, 177, 255 }, /// ROCK_0
                {   0,   0,   0,   0 }  /// TRANSPARENT
        };

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
        void updateSandParticle (int _x, int _y, int _posInVector, Timestep _dt);
        void updateWaterParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateRockParticle (int _x, int _y, int _posInVector, Timestep _dt);

        void setWaterParticleDirection(Particle& _particle, const Vec2f& _particlePosition);

        Color particleTypeToColor(const ParticleType& _particle);
        void generateParticles(const Vec2f& _mousePos);
        void generateWithBrush(const Vec2f& _mousePos);
        void removeParticles(const Vec2f& _mousePos);

        bool isInBounds(int _x, int _y);
        int calcVecPos(int _x, int _y);
        bool isEmpty(int _x, int _y);

        void writeParticle(int _x, int _y, const Particle& _particle);
};

#endif //TESTGAME_H
