#pragma once

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <entt.hpp>
#include <engine/util/Timestep.h>

namespace engine {

    /// This class acts as the container of a whole bunch of objects or as a map or place.
    class GameObject;
    class Scene {
        private:
            entt::registry gameObjectsRegistry;
        public:
            Scene();
            ~Scene();

            GameObject createGameObject(const std::string& _name = "");

        public:
            void onUpdate(Timestep _dt);
            void onFixUpdate(Timestep _fixedDt);
            void onRender(Timestep _dt);

            friend class GameObject;
    };
}

#endif //ENGINE_SCENE_H
