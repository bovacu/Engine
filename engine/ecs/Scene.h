#pragma once

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <entt.hpp>
#include <engine/util/Delta.h>

namespace engine {

    class GameObject;

    class Scene;
    typedef std::shared_ptr<Scene> ScenePtr;
    /// This class acts as the container of a whole bunch of objects or as a map or place.
    class Scene {
        private:
            entt::registry gameObjectsRegistry;
            int viewportWidth = 0, viewportHeight = 0;
            std::string name;

        public:
            explicit Scene(const std::string& _name = "");
            ~Scene();

            GameObject createGameObject(const std::string& _name = "");

        public:
            void onUpdate(Delta _dt);
            void onFixUpdate(Delta _fixedDt);
            void onRender(Delta _dt);
            void onImGuiRender(Delta _dt);
            void onViewportResize(int _width, int _height);

            entt::registry& getGameObjectsRegistry() { return this->gameObjectsRegistry; }
            const std::string& getName() const { return this->name; }

            static ScenePtr create(const std::string& _name = "");

            friend class GameObject;
    };
}

#endif //ENGINE_SCENE_H
