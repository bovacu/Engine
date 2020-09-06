#pragma once

#ifndef ENGINE_PHYSICS_H
#define ENGINE_PHYSICS_H

#include <box2d/box2d.h>

namespace engine {

    class Physics {
        private:
            static b2World world;

        public:
            static void setGravity(const Vec2f& _gravity);
            static Vec2f getGravity();
            static b2World& getWorld();
            static void step();
    };

}

#endif //ENGINE_PHYSICS_H
