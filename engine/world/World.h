#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <engine/components/physics/PhysicsBody.h>
#include <engine/gameObject/GameObject.h>

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
            void update(float _dt);
            void postUpdate(float _postDt);
            void fixedUpdate(float _fixDt);

            bool addGameObject(GameObjectPtr& _gameObject, bool _isStatic = true);
            bool removeGameObject(GameObjectPtr& _gameObject);

            void clearWorld();

            void debugWorld();
            void setDebuggingFunction(const DebuggingFunction& _debuggingFunction);

        private:
            void detectCollisions();
    };

}

#endif // WORLD_H