#pragma once

#ifndef TEST_GAME_H
#define TEST_GAME_H

#define MAX_BRUSH_THICKNESS 20
#define MAX_ZOOM_LEVEL      15.f

#define INFINITE_LIFE_TIME -1.f
#define MAX_WATER_LIFE      8.f
#define MIN_WATER_LIFE      4.f

#include <engine/main/Engine.h>

using namespace engine;

class TestGame : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE,                                                                              /// NOTHING
                            SAND    , GUNPOWDER , SALT  ,                                                               /// DUSTS
                            WATER   , ACID      , LAVA  , POISON_L  ,                                                   /// LIQUIDS
                            STONE   , WOOD      , ICE   , SNOW      ,   STEEL   ,   WAX , DIRT  ,                       /// SOLIDS
                            STEAM   , SMOKE     , GAS   , POISON_G  ,                                                   /// GASES
                            CLOUD   , FIRE      , PLANT ,                                                               /// VARIOUS
                            FUSE                                                                                        /// UTILS
        };

        enum Tool { DRAW, ERASE, ZOOM };
        float gravity = 10.f;

        struct Particle {
            Vec2f velocity  {0.0f, 0.0f};
            Color color = {   0,   0,   0,   0 };
            ParticleType type = NONE_PARTICLE;
            bool canUpdate = true;
            float lifeTimer = 0.0f;
            float lifeTime = INFINITE_LIFE_TIME;
            int lastHeight = 0;
            float temperature;
        };

        struct ReactionInfo {
            Probability prob;
            bool reactionExists = false;
        };

    private:
        engine::OrthographicCameraController cameraController;
        glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        Texture2DPtr proceduralTexture, circleTexture;
        int totalOfPixels, drawnPixels;
        ImGuiTexture2DPtr pauseTexture, resumeTexture, advanceTexture, oneFrameTexture, drawTexture, eraseTexture,
                            zoomTexture, pointTexture;
        Particle* particles;
        Application& app;

        bool play = true, oneStep = false, justPressed = true;
        ParticleType selectedParticle = SAND;

        engine::Random random;

        int particlesUpdating = 0;
        int particlesInSecond = 0;
        float particlesUpdateTimer = 0.0f;

        static Particle noneParticle, sandParticle, waterParticle, stoneParticle, acidParticle, dirtParticle;
        static int textureWidth, textureHeight;

        Color PARTICLE_COLORS[19] = {
                { 202, 188, 145, 255 }, /// SAND_0
                { 194, 178, 128, 255 }, /// SAND_1
                { 186, 168, 111, 255 }, /// SAND_2
                { 177, 157,  94, 255 }, /// SAND_3
                { 166, 145,  80, 255 }, /// SAND_4
                {  90, 188, 216, 125 }, /// WATER_0
                { 170, 164, 157, 255 }, /// ROCK_0
                { 149, 141, 133, 255 }, /// ROCK_1
                { 123, 113, 103, 255 }, /// ROCK_2
                { 255, 254,  47, 255 }, /// ACID_0
                { 255, 229,  47, 255 }, /// ACID_1
                { 199, 213, 224, 255 }, /// STEAM_0
                {  43,  50,  48, 255 }, /// SMOKE_0
                {  39,  45,  43, 255 }, /// SMOKE_1
                {  34,  40,  38, 255 }, /// SMOKE_2
                {   0,   0,   0,   0 }, /// TRANSPARENT
                {  83,  66,  41, 255 }, /// DIRT_0
                {  69,  55,  35, 255 }, /// DIRT_1
                {  55,  44,  28, 255 }, /// DIRT_2
        };

        int whatToDoWithUnfittingDrops = 0; /// 0 = leave them alone, 1 = remove them, 2 = evaporate them
        float weatherConditions[5] = {
            0       ,   /// Wind
            20      ,   /// Temperature (Celsius)
            0       ,   /// Rain
            0       ,   /// Snow
            9.8f    ,   /// Gravity
        };

        ParticleType rainType = WATER;

        Color backgroundColor = Color::Black;

        Tool usingTool = DRAW;
        int brushSize = 10;
        int brushCircleWH = brushSize;
        float zoomLevel = 3.0f;
        Color zoomDotColor = Color::Red;
        float zoomImageWidth = 32, zoomImageHeight = 32;

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
        void updateStoneParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateAcidParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateDirtParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void handleUnfittedDrops(int _x, int _y, int _vecPos, float _dt);

        void wind();
        void rain();
        void snow();

        Color particleTypeToColor(const ParticleType& _particle);
        static const char* particleTypeToName(const ParticleType& _type);
        void checkForShortcuts();

        void generateWithBrush(const Vec2f& _mousePos);
        void removeParticles(const Vec2f& _mousePos);
        void zoomParticles(const Vec2f& _pos);

        bool isInBounds(int _x, int _y);
        int calcVecPos(int _x, int _y);
        bool isEmpty(int _x, int _y);
        bool isSurrounded(int _x, int _y);
        void activateNeighbours(int _x, int _y, int _width);

        bool is(int _x, int _y, const ParticleType& _particle);

        bool isSolid(const ParticleType& _type);

        void generateParticles(const Vec2f& _mousePos);
        void writeParticle(int _x, int _y, const Particle& _particle);
        void writeParticle(int _x, int _y,int _vecPos, const Particle& _particle);

        void imGuiAppWindow(engine::Timestep _dt);
        void imGuiInfo(engine::Timestep _dt);
        void imGuiControllerWindow(engine::Timestep _dt);
        void imGuiConditions(engine::Timestep _dt);
        void imGuiWeather(engine::Timestep _dt);
        void imGuiDrawingWindow(engine::Timestep _dt);
        void imGuiMaterials(engine::Timestep _dt);
        void imGuiSettings(engine::Timestep _dt);

        void imGuiWorldSizePopUp(engine::Timestep _dt);

        static float probValues(const ParticleType& _firstParticle, const ParticleType& _secondParticle);
        ReactionInfo reactions(const Vec2i& _posA, const Vec2i& _posB, Particle& _particleA, const Particle& _particleB);

        bool onMouseScrolled(MouseScrolledEvent& _e);

        void generateCircleTexture();
        void circleMidPoints(const Vec2i& _center, int _radius, const Color& _color);
        void circlePoints(const Vec2i& _center, const Vec2i& _pos, const Color& _color);
        Vec2i randomPointInsideCircle(const Vec2i& _mousePos, int _radius);
};

#endif //TEST_GAME_H
