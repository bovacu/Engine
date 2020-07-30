#pragma once

#ifndef ENGINE_GAME_OBJECT_H
#define ENGINE_GAME_OBJECT_H

#include <entt.hpp>
#include <engine/ecs/Scene.h>

namespace engine {

    class GameObject {
        private:
            entt::entity    ref { entt::null };
            Scene*          scene = nullptr;

        public:
            GameObject() = default;
            GameObject(entt::entity _ref, Scene* _scene) : ref(_ref), scene(_scene) {  }
            GameObject(const GameObject& _gameObject) = default;

            template<typename T, typename... Args>
            T& addComponent(Args&&... args) {
                return this->scene->gameObjectsRegistry.emplace<T>(this->ref, std::forward<Args>(args)...);
            }

            template<typename T>
            T& getComponent() {
                return this->scene->gameObjectsRegistry.get<T>(this->ref);
            }

            template<typename T>
            bool hasComponent() {
                return this->scene->gameObjectsRegistry.has<T>(this->ref);
            }

            template<typename T>
            void removeComponent() {
                this->scene->gameObjectsRegistry.remove<T>(this->ref);
            }

            explicit operator bool() const { return this->ref != entt::null; }
    };



}

#endif //ENGINE_GAME_OBJECT_H
