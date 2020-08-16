#pragma once

#ifndef ENGINE_SCRIPTABLEOBJECT_H
#define ENGINE_SCRIPTABLEOBJECT_H


#include <engine/ecs/GameObject.h>
#include <engine/ecs/EngineAPI.h>

namespace engine {

    class ScriptableObject {
        friend class Scene;

        public:
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

        private:
            GameObject gameObject;
    };

}

#endif //ENGINE_SCRIPTABLEOBJECT_H
