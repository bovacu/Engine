#pragma once

#ifndef TEST_GAME_H
#define TEST_GAME_H

#define MAX_BRUSH_THICKNESS 60
#define MAX_ZOOM_LEVEL      15.f

#define INFINITE_LIFE_TIME -1.f
#define MAX_WATER_LIFE      8.f
#define MIN_WATER_LIFE      4.f
#define MAX_STEAM_LIFE      6.f
#define MIN_STEAM_LIFE      4.f

#define WATER_SPREAD_RATE   4
#define ACID_SPREAD_RATE    3
#define LAVA_SPREAD_RATE    1

#include <engine/main/Engine.h>

using namespace engine;

class Safator : public engine::Layer {

    public:
        enum ParticleType { NONE_PARTICLE,                                                                                  /// NOTHING
                            SAND    , GUNPOWDER , SALT  , ASH       ,                                                       /// DUSTS
                            WATER   , ACID      , LAVA  , POISON_L  ,                                                       /// LIQUIDS
                            STONE   , WOOD      , ICE   , FROST     ,   SNOW     ,   STEEL   ,   WAX , DIRT  , OBSIDIAN ,   /// SOLIDS
                            STEAM   , SMOKE     , GAS   , POISON_G  ,                                                       /// GASES
                            CLOUD   , FIRE      , PLANT ,                                                                   /// VARIOUS
                            FUSE    , CRYOGENER ,                                                                           /// UTILS
        };
        enum Tool { DRAW, ERASE, ZOOM };

        struct MapInfo {
            uint32_t width, height;
            uint32_t numberOfComponents;
            unsigned char* pixels;
        };

        struct Particle {
            Vec2f velocity  {0.0f, 0.0f};
            Color color = {   0,   0,   0,   0 };
            ParticleType type = NONE_PARTICLE;
            bool canUpdate = true;
            bool updatedThisFrame = false;
            float lifeTimer = 0.0f;
            float lifeTime = INFINITE_LIFE_TIME;
            int lastHeight = 0;
            float temperature;
        };
        struct ReactionInfo {
            Probability prob;
            bool reactionExists = false;
        };
        struct DrawInfo {
            int brushSize = 10;
            int brushCircleWH = brushSize;
        } drawInfo;
        struct ZoomInfo {
            float level = 3.0f;
            Color dotColor = Color::Red;
            float imageWidth = 32, imageHeight = 32;
        } zoomInfo;
        struct EraseInfo {
            Color circleColor = Color::Red;
        } eraseInfo;

    private:
        engine::OrthographicCameraController    cameraController;
        Color                                   backgroundColor = Color::Black;
        engine::FrameBufferPtr                  frameBuffer;

        Texture2DPtr                            worldTexture,
                                                circleTexture;

        int                                     totalOfPixels = 0,
                                                drawnPixels = 0;

        ImGuiTexture2DPtr                       pauseTexture,
                                                resumeTexture,
                                                advanceTexture,
                                                oneFrameTexture,
                                                drawTexture,
                                                eraseTexture,
                                                zoomTexture;

        Particle*                               particles = nullptr;
        Application&                            app;

        ParticleType                            selectedParticle = SAND;
        ParticleType                            rainType = WATER;
        Tool                                    tool = DRAW;

        bool                                    play = true,
                                                oneStep = false;

        engine::Random                          random;

        int                                     whatToDoWithUnfittingDrops = 0; /// 0 = remove them, 1 = evaporate them, 2 = leave them alone
        int                                     whatToDoWithUnfittingGas = 1;   /// 0 = remove them, 1 = condensate them, 2 = leave them alone

        int                                     particlesUpdating = 0;
        int                                     particlesInSecond = 0;
        float                                   particlesUpdateTimer = 0.0f;

        Particle                                noneParticle,
                                                sandParticle,
                                                waterParticle,
                                                stoneParticle,
                                                acidParticle,
                                                dirtParticle,
                                                iceParticle,
                                                saltParticle,
                                                gunpowderParticle,
                                                lavaParticle,
                                                obsidianParticle,
                                                cryoParticle,
                                                frostParticle,
                                                steamParticle,
                                                smokeParticle,
                                                poisonGParticle,
                                                ashParticle,
                                                gasParticle,
                                                fireParticle;

        int                                     textureWidth = 0,
                                                textureHeight = 0;

        Color                                   PARTICLE_COLORS[39] = {
                                                    { 202, 188, 145, 255 }, /// SAND_0
                                                    { 194, 178, 128, 255 }, /// SAND_1
                                                    { 186, 168, 111, 255 }, /// SAND_2
                                                    { 177, 157,  94, 255 }, /// SAND_3
                                                    { 166, 145,  80, 255 }, /// SAND_4
                                                    {  72,  86, 238, 255 }, /// WATER_0
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
                                                    { 205, 241, 249, 255 }, /// ICE_0
                                                    { 190, 238, 249, 255 }, /// ICE_1
                                                    { 238, 243, 229, 255 }, /// SALT_0
                                                    { 245, 248, 239, 255 }, /// SALT_1
                                                    {  70,  70,  80, 255 }, /// GUNPOWDER_1
                                                    {  56,  56,  64, 255 }, /// GUNPOWDER_2
                                                    {  45,  45,  51, 255 }, /// GUNPOWDER_3
                                                    { 248,  58,  12, 255 }, /// LAVA_0
                                                    { 220,  58,  12, 255 }, /// LAVA_0
                                                    {  59,  30,  58, 255 }, /// OBSIDIAN_0
                                                    {  59,  26,  58, 255 }, /// OBSIDIAN_1
                                                    {  59,  19,  58, 255 }, /// OBSIDIAN_2
                                                    {  63, 208, 212, 255 }, /// CRYOGENER_0
                                                    {  63, 191, 212, 255 }, /// CRYOGENER_0
                                                    {  84, 142, 249, 255 }, /// FROST_0
                                                    {  73, 134, 212, 255 }, /// FROST_1
                                                    { 127,   1, 254, 122 }, /// POISON_G_0
                                                    { 111, 118, 120, 255 }, /// ASH_0
                                                    { 111, 108, 120, 255 }, /// ASH_1
                                                    { 108, 199, 108, 255 }, /// GAS_0
                                                };

        float                                   weatherConditions[5] = {
                                                    0       ,   /// Wind
                                                    20      ,   /// Temperature (Celsius)
                                                    0       ,   /// Rain
                                                    0       ,   /// Snow
                                                    9.8f    ,   /// Gravity
                                                };

    public:
        Safator();
        ~Safator()                                  override = default;

        void onInit()                               override;
        void onEvent(engine::Event& _e)             override;
        void onUpdate(engine::Timestep _dt)         override;
        void onFixedUpdate(engine::Timestep _dt)    override;
        void onRender(engine::Timestep _dt)         override;
        void onImGuiRender(engine::Timestep _dt)    override;
        void onEnd()                                override;

    private:
        void initSimulationWorld();
        void updateAllParticles(int _x, int _y, int _posInVector, const ParticleType& _type, Timestep _dt);
        void updateDirtParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void updateFrostParticle(int _x, int _y, int _posInVector, Timestep _dt);
        void handleUnfittedDrops(int _x, int _y, int _vecPos, float _dt);
        void handleUnfittedGases(int _x, int _y, int _vecPos, float _dt);

        void updateCommonDusts(int _x, int _y, int _posInVector, Timestep _dt);
        void updateCommonLiquids(int _x, int _y, int _posInVector, int _spreadRate, Timestep _dt);
        void updateCommonGases(int _x, int _y, int _posInVector, Timestep _dt);

        bool reactWithFourNeighbours(int _x, int _y, Particle& _tempA, Particle& _tempB, ReactionInfo& _ri);

        void wind();
        void rain();
        void snow();

        Color particleTypeToColor(const ParticleType& _particle);
        static const char* particleTypeToName(const ParticleType& _type);

        bool onMouseScrolled(MouseScrolledEvent& _e);
        void checkForShortcuts();

        void removeWithBrush(const Vec2i& _mousePos);
        void generateWithBrush(const Vec2i& _mousePos);
        void removeParticles(const Vec2i& _mousePos);
        void zoomParticles(const Vec2i& _pos);

        int calcVecPos(int _x, int _y);
        void activateNeighbours(int _x, int _y);

        bool isEmpty(int _x, int _y);
        bool isEmptyForGases(int _x, int _y, const ParticleType& _type);
        bool isSurrounded(int _x, int _y);
        bool isInBounds(int _x, int _y);
        bool is(int _x, int _y, const ParticleType& _particle);

        /// They are both static as CLion suggested
        static bool isSolid(const ParticleType& _type);
        static bool notUpdatable(const ParticleType& _particleType);

        void generateParticles(const Vec2i& _mousePos);
        void generateSpecificParticle(const Vec2i& _pos, const ParticleType& _type);
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
        void imGuiSaveWorld(engine::Timestep _dt);
        void imGuiLoadWorld(engine::Timestep _dt);

        static float probValues(const ParticleType& _firstParticle, const ParticleType& _secondParticle);
        ReactionInfo reactions(const Vec2i& _posA, const Vec2i& _posB, Particle& _particleA, Particle& _particleB);

        void generateCircleTexture();
        void circleMidPoints(const Vec2i& _center, int _radius, const Color& _color);
        void circlePoints(const Vec2i& _center, const Vec2i& _pos, const Color& _color);
        Vec2i randomPointInsideCircle(const Vec2i& _mousePos, int _radius);

        void saveWorld(const std::string& _worldName);
        void loadWorld(const std::string& _worldName);
};

#endif //TEST_GAME_H
