#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <engine/components/physics/PhysicsBody.h>
#include <engine/gameObject/GameObject.h>
#include <engine/render/Render2D.h>

namespace engine {

    class World {
        typedef std::function<void ()> DebuggingFunction;

        public:
            std::vector<GameObjectPtr> objects;

            float timeStep;
            Vec2f gravity;

            DebuggingFunction debuggingFunction;

        public:
            explicit World(float _timeStep = 1.0f / 60.0f, const Vec2f& _gravity = {0, 0});
            
            void init();
            void update(Timestep _dt);
            void fixedUpdate(Timestep _fixDt);

            bool addGameObject(GameObjectPtr& _gameObject, bool _isStatic = true);
            bool removeGameObject(GameObjectPtr& _gameObject);

            void clearWorld();

            void debugWorld(const OrthographicCamera& _camera);
            void setDebuggingFunction(const DebuggingFunction& _debuggingFunction);

        private:
            void detectCollisions();
    };

}

#endif // WORLD_H