#pragma once

#ifndef TESTGAME_H
#define TESTGAME_H

#define MAX_BRUSH_THICKNESS 20

#define IMGUI_DEBUGGING     0
#define IMGUI_CONTROLLER    1
#define IMGUI_DRAWING       2
#define IMGUI_CRAFTER       3

#define INFINITE_LIFE_TIME -1.f
#define MAX_WATER_LIFE      8.f
#define MIN_WATER_LIFE      4.f

#include <engine/main/Engine.h>

using namespace engine;

class TestGame : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE, SAND, WATER, ROCK, ACID, STEAM, SMOKE };
        enum Tool { DRAW, ERASE };
        float gravity = 10.f;

        struct Particle {
            Vec2f velocity  {0.0f, 0.0f};
            Color color = {   0,   0,   0,   0 };
            ParticleType type = NONE_PARTICLE;
            bool canUpdate = true;
            float lifeTimer = 0.0f;
            float lifeTime = INFINITE_LIFE_TIME;
            int lastHeight = 0;
        };

    private:
        engine::OrthographicCameraController cameraController;
        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        Texture2DPtr proceduralTexture;
        ImGuiTexture2DPtr pauseTexture, resumeTexture, advanceTexture, oneFrameTexture, drawTexture, eraseTexture;
        ImGuiTexture2DPtr enabledTexture, disabledTexture;
        Particle* particles;
        Application& app;

        bool play = true, oneStep = false;
        ParticleType selectedParticle = SAND;

        Tool usingTool = DRAW;
        int brushSize = 10;

        engine::Random random;

        int particlesUpdating = 0;
        int particlesInSecond = 0;
        float particlesUpdateTimer = 0.0f;

        static Particle noneParticle, sandParticle, waterParticle, rockParticle, acidParticle;
        static int textureWidth, textureHeight;

        Color PARTICLE_COLORS[16] = {
                { 202, 188, 145, 255 }, /// SAND_0
                { 194, 178, 128, 255 }, /// SAND_1
                { 186, 168, 111, 255 }, /// SAND_2
                { 177, 157,  94, 255 }, /// SAND_3
                { 166, 145,  80, 255 }, /// SAND_4
                {  90, 188, 216, 125 }, /// WATER_0
                { 170, 164, 157, 255 }, /// ROCK_0
                { 149, 141, 133, 255 }, /// ROCK_1
                { 123, 113, 103, 255 }, /// ROCK_2
                { 176, 191,  26, 255 }, /// ACID_0
                { 203, 227,  21, 255 }, /// ACID_1
                { 199, 213, 224, 255 }, /// STEAM_0
                {  43,  50,  48, 255 }, /// SMOKE_0
                {  39,  45,  43, 255 }, /// SMOKE_1
                {  34,  40,  38, 255 }, /// SMOKE_2
                {   0,   0,   0,   0 }  /// TRANSPARENT
        };

        bool tools[4] = {
                true,       /// Debugging
                true,       /// Controller
                true,       /// Drawing
                false,       /// Crafter
        };

        int whatToDoWithUnfittingDrops = 0; /// 0 = leave them alone, 1 = remove them, 2 = evaporate them

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
        void updateSandParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateWaterParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateRockParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateAcidParticle(int _x, int _y, int _posInVector, Timestep _dt);

        Color particleTypeToColor(const ParticleType& _particle);
        void generateParticles(const Vec2f& _mousePos);
        void generateWithBrush(const Vec2f& _mousePos);
        void removeParticles(const Vec2f& _mousePos);

        bool isInBounds(int _x, int _y);
        int calcVecPos(int _x, int _y);
        bool isEmpty(int _x, int _y);
        bool is(int _x, int _y, const ParticleType& _particle);
        bool isSurrounded(int _x, int _y);
        void activateNeighbours(int _x, int _y, int _width);

        void writeParticle(int _x, int _y, const Particle& _particle);
        void writeParticle(int _x, int _y,int _vecPos, const Particle& _particle);

        void imGuiToolEnablerWindow(engine::Timestep _dt);
        void imGuiDrawingWindow(engine::Timestep _dt);
        void imGuiControllerWindow(engine::Timestep _dt);
        void imGuiCrafterWindow(engine::Timestep _dt);

        static float probValues(const ParticleType& _firstParticle, const ParticleType& _secondParticle);
        bool reactions(const Vec2i& _posA, const Vec2i& _posB, Particle& _particleA, const Particle& _particleB);
        void handleUnfittedDrops(int _x, int _y, int _vecPos, float _dt);
};

#endif //TESTGAME_H
