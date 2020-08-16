#if defined(COMPILE_TEST)
#pragma once

#ifndef ENGINE_STRESSTEST_H
#define ENGINE_STRESSTEST_H

#include <engine/main/Engine.h>

using namespace engine;
class StressTest : public Layer {

        struct Position_Index {
            Vec2f position;
            int textureIndex;
            Position_Index(const Vec2f& _position, int _index) : position(_position), textureIndex(_index) {  }
        };

        struct StressTestInfo {
            int numberOfSprites = 100;
            std::vector<Position_Index> positionsAndTexture;
            bool rotate = false;
            float rotation = 0.0f;
        } stressTestInfo;

        struct IndependenceInfo {
            int carSpeed0 = 50;
            int carSpeed1 = 75;
            int carSpeed2 = 100;
            int carSpeed3 = 125;
        } independenceInfo;

        struct EventInfo {
            bool moving = false;
            bool minimizing = false;
            bool resizing = false;

            std::vector<int> mouseClicks;
            int scroll = 0;

            std::vector<KeyCode> keys;

            void restart() {
                moving = false;
                minimizing = false;
                resizing = false;

                mouseClicks.clear();
                scroll = 0;

                keys.clear();
            }
        } eventInfo;

    private:
        OrthographicCameraController        cameraController;
        Application&                        app;

        bool                                tests[4] = {
                                                false,          // Stress
                                                false,          // Events
                                                false,          // Frame Independence
                                                false,          // Collisions
                                            };
        Texture2DPtr                        stressSpriteSheet, independentSpriteSheet, physicsSpriteSheet;
        TextureRegionPtr                    stressSubTextures[12];
        TextureRegionPtr                    independentTextures[4];
        TextureRegionPtr                    physicsTextures[4];

        GameObjectPtr                       cars[4];
        GameObjectPtr                       physics[5];
        World                               worldCars, worldPhysics;

        Random                              rand;
        bool                                vSync = false;
        FrameBufferPtr                      frameBufferPtr;

    public:
        StressTest();
        ~StressTest()                       override = default;

        void onInit()                       override;
        void onEvent(Event& _e)             override;
        void onUpdate(Timestep _dt)         override;
        void onFixedUpdate(Timestep _dt)    override;
        void onRender(Timestep _dt)         override;
        void onImGuiRender(Timestep _dt)    override;
        void onEnd()                        override;

    private:
        void recalculateStressTest();
        bool mouseScrollEvent(MouseScrolledEvent& _e);
        bool windowResizedEvent(WindowResizedEvent& _e);
        bool windowMovedEvent(WindowMovedEvent& _e);
        bool windowMinimizedEvent(WindowMinimizedEvent _e);

        void initCars();
        void initPhysics();
};

#endif //ENGINE_STRESSTEST_H
#endif