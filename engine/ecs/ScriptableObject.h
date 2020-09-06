#pragma once

#ifndef ENGINE_SCRIPTABLEOBJECT_H
#define ENGINE_SCRIPTABLEOBJECT_H

#include <engine/ecs/EngineAPI.h>
#include <engine/ecs/GameObject.h>

namespace engine {

    class ScriptableObject {
        private:
            GameObject gameObject;

        public:
            virtual void onInit() {  }
            virtual void onUpdate(Delta _dt) {  }
            virtual void onFixedUpdate(Delta _fixedDt) {  }
            virtual void onEnd() {  }

            GameObject newGameObject() {
                return this->gameObject.scene->createGameObject();
            }

            GameObject newGameObject(const Vec2f& _position) {
                return this->gameObject.newGameObject(_position);
            }

            void destroyGameObject(const GameObject& _gameObject) {
                this->gameObject.destroyGameObject(_gameObject);
            }

            GameObject& getGameObject() {
                return this->gameObject;
            }

            template<typename T>
            T& addComponent() {
                return this->gameObject.addComponent<T>();
            }

            template<typename T>
            T& getComponent() {
                return this->gameObject.getComponent<T>();
            }

            template<typename T>
            bool hasComponent() {
                return this->gameObject.hasComponent<T>();
            }

            template<typename T>
            void removeComponent() {
                this->gameObject.removeComponent<T>();
            }

            friend class Scene;
    };

}

#endif //ENGINE_SCRIPTABLEOBJECT_H
